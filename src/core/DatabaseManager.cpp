#include "DatabaseManager.hpp"

#include <QFile>
#include <QLoggingCategory>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantMap>

Q_LOGGING_CATEGORY(lcDb, "bl.core.db")

namespace bl::core {

DatabaseManager::DatabaseManager(const QString &dbName) : _lastInsertedID{0} {
  static const QString dbDriverName = "QSQLITE";

  _db = QSqlDatabase::addDatabase(dbDriverName);
  _db.setDatabaseName(dbName);
  qCInfo(lcDb) << "Database configured:" << dbName;
}

DatabaseManager::~DatabaseManager() { close(); }

bool DatabaseManager::open() {
  if (_db.open()) {
    qCInfo(lcDb) << "Database opened:" << _db.databaseName();
    return true;
  }

  qCWarning(lcDb) << "Failed to open database:" << _db.lastError().text();
  return false;
}

void DatabaseManager::close() {
  if (_db.isOpen()) {
    qCInfo(lcDb) << "Database closed";
    _db.close();
  }
}

bool DatabaseManager::runScript(const QString &scriptFileName) {
  QFile file(scriptFileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCWarning(lcDb) << "Cannot open script" << scriptFileName << ":"
                    << file.errorString();
    return false;
  }

  qCInfo(lcDb) << "Running script:" << scriptFileName;

  QTextStream script(&file);
  trimRun(script);

  file.close();

  return true;
}

bool DatabaseManager::runQuery(const QString &sqlQuery, QString *error) {
  QSqlQuery query{_db};
  if (!_db.transaction()) {
    qCWarning(lcDb) << "Failed to begin transaction:" << _db.lastError().text();
    if (error) {
      *error = _db.lastError().text();
    }
    return false;
  }

  bool ret = query.exec(sqlQuery);

  if (ret) {
    _db.commit();
  } else {
    qCWarning(lcDb) << "Query failed:" << query.lastError().text();
    if (error) {
      *error = query.lastError().text();
    }

    _db.rollback();
  }

  return ret;
}

QList<QVariantMap> DatabaseManager::runQuery(const SqlQueryBuilder &sqlQuery,
                                             QString *error) {
  QSqlQuery query{_db};
  if (!_db.transaction()) {
    qCWarning(lcDb) << "Failed to begin transaction:" << _db.lastError().text();
    if (error) {
      *error = _db.lastError().text();
    }
    return {};
  }

  auto params = sqlQuery.getValues();
  QString sql = sqlQuery.build();

  if (!query.prepare(sql)) {
    qCWarning(lcDb) << "Prepare failed:" << query.lastError().text()
                    << "sql:" << sql;
    if (error) {
      *error = query.lastError().text();
    }
    _db.rollback();
    return {};
  }

  for (const auto &param : params) {
    query.addBindValue(param);
  }

  qCDebug(lcDb) << "Executing:" << sql << "params:" << params;

  if (query.exec()) {
    _db.commit();
  } else {
    qCWarning(lcDb) << "Query failed:" << query.lastError().text();
    if (error) {
      *error = query.lastError().text();
    }

    _db.rollback();
  }

  auto lastInsertedID = query.lastInsertId();
  _lastInsertedID = lastInsertedID.isValid() ? lastInsertedID.toLongLong() : 0;

  return getDataFromQuery(query);
}

qint64 DatabaseManager::lastInsertedID() const { return _lastInsertedID; }

void DatabaseManager::trimRun(QTextStream &script) {
  QString currentCommand;

  while (!script.atEnd()) {
    QString line = script.readLine();
    if (line.isEmpty() || line.startsWith("--")) {
      continue;
    }

    if (line.startsWith(" ")) {
      auto trimmedLine = line.trimmed();
      auto endIndex = trimmedLine.indexOf("--");
      if (endIndex != -1) {
        currentCommand += trimmedLine.left(endIndex) + " ";
      } else {
        currentCommand += trimmedLine + " ";
      }

    } else {
      currentCommand += line + " ";
      if (line.trimmed().endsWith(";")) {
        QString error;
        if (!runQuery(currentCommand, &error)) {
          qCWarning(lcDb) << "Script statement failed:" << error;
        }

        currentCommand.clear();
      }
    }
  }
}

QList<QVariantMap> DatabaseManager::getDataFromQuery(QSqlQuery &query) {
  QList<QVariantMap> ret;
  query.record();

  while (query.next()) {
    QSqlRecord record = query.record();
    QVariantMap row;
    for (int i = 0; i < record.count(); ++i) {
      QString columnName = query.record().fieldName(i);
      row[columnName] = query.value(i);
    }

    ret.append(std::move(row));
  }

  qCDebug(lcDb) << "Query returned" << ret.size() << "rows";

  return ret;
}

} // namespace bl::core
