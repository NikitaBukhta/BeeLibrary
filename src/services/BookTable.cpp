#include "BookTable.hpp"
#include "core/SqlQueryBuilder.hpp"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcBookTable, "bl.services.books")

namespace bl::services {

const QString BookTable::_tableName = "books";

BookTable::BookTable(std::weak_ptr<core::DatabaseManager> db)
    : _db{db.lock()} {}

void BookTable::getAllBooks(QList<BookDTO> &out) {
  core::SqlQueryBuilder query;
  QString error;

  out.clear();

  query.select({"id", "title", "author", "year", "isbn"}).from(_tableName);

  auto data = _db->runQuery(query, &error);
  out.reserve(data.count());

  BookDTOMapper mapper;
  for (const auto &it : std::as_const(data)) {
    out.emplaceBack(std::move(mapper.map(it)));
  }

  qCInfo(lcBookTable) << "Loaded" << out.size() << "books";
}

qint64 BookTable::addBook(const BookDTO &book) {
  core::SqlQueryBuilder query;
  QString error;

  query.insertInto(_tableName, {"title", "author", "year", "isbn"})
      .values({book.title, book.author, book.year, book.isbn});

  _db->runQuery(query, &error);

  qint64 id = _db->lastInsertedID();
  if (id > 0)
    qCInfo(lcBookTable) << "Added book id:" << id << "title:" << book.title;
  else
    qCWarning(lcBookTable) << "Failed to add book:" << book.title
                           << "error:" << error;

  return id;
}

bool BookTable::updateBook(const BookDTO &book) {
  core::SqlQueryBuilder query;
  QString error;

  query.update(_tableName)
      .set({"title", "author", "year", "isbn"})
      .where("id = ?")
      .values({book.title, book.author, book.year, book.isbn, book.id});

  _db->runQuery(query, &error);

  if (error.isEmpty()) {
    qCInfo(lcBookTable) << "Updated book id:" << book.id
                        << "title:" << book.title;
    return true;
  }

  qCWarning(lcBookTable) << "Failed to update book id:" << book.id
                         << "error:" << error;
  return false;
}

bool BookTable::deleteBook(qint64 id) {
  core::SqlQueryBuilder query;
  QString error;

  query.deleteFrom(_tableName).where("id = ?").values({id});

  _db->runQuery(query, &error);

  if (error.isEmpty()) {
    qCInfo(lcBookTable) << "Deleted book id:" << id;
    return true;
  }

  qCWarning(lcBookTable) << "Failed to delete book id:" << id
                         << "error:" << error;
  return false;
}

bool BookTable::isbnExists(const QString &isbn, qint64 excludeId) {
  core::SqlQueryBuilder query;
  QString error;

  query.selectCount()
      .from(_tableName)
      .where("isbn = ? AND id != ?")
      .values({isbn, excludeId});

  auto data = _db->runQuery(query, &error);

  if (data.isEmpty())
    return false;

  return data.first().value("COUNT(*)").toInt() > 0;
}

} // namespace bl::services
