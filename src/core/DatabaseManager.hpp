#ifndef BEELIBRARY_CORE_DATABASEMANAGER_HPP
#define BEELIBRARY_CORE_DATABASEMANAGER_HPP

#include "SqlQueryBuilder.hpp"

#include <QCoreApplication>
#include <QString>
#include <QtSql/QSqlDatabase>

namespace bl::core {

class DatabaseManager {
public:
  DatabaseManager(const QString &dbName);
  ~DatabaseManager();

  bool open();
  void close();
  bool runScript(const QString &scriptFileName);
  bool runQuery(const QString &sqlQuery, QString *error = nullptr);
  QList<QVariantMap> runQuery(const SqlQueryBuilder &sqlQuery,
                              QString *error = nullptr);
  qint64 lastInsertedID() const;

private:
  void trimRun(QTextStream &script);
  QList<QVariantMap> getDataFromQuery(QSqlQuery &query);

private:
  QSqlDatabase _db;
  qint64 _lastInsertedID;
};

} // namespace bl::core

#endif // BEELIBRARY_CORE_DATABASEMANAGER_HPP
