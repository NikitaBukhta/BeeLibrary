#include "BookTable.hpp"
#include "core/SqlQueryBuilder.hpp"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcBookTable, "bl.services.books")

namespace bl::services {

const QString BookTable::kTableName = QStringLiteral("books");

BookTable::BookTable(std::shared_ptr<core::DatabaseManager> db)
    : _db{std::move(db)} {}

QList<BookDTO> BookTable::getAllBooks() {
  core::SqlQueryBuilder query;
  QString error;

  query.select({"id", "title", "author", "year", "isbn"})
      .from(kTableName)
      .orderBy("title");

  auto data = _db->select(query, &error);

  QList<BookDTO> result;
  result.reserve(data.size());
  for (const auto &row : std::as_const(data))
    result.emplaceBack(BookDTO::fromMap(row));

  qCInfo(lcBookTable) << "Loaded" << result.size() << "books";
  return result;
}

qint64 BookTable::addBook(const BookDTO &book) {
  core::SqlQueryBuilder query;
  QString error;

  query.insertInto(kTableName, {"title", "author", "year", "isbn"})
      .values({book.title, book.author, book.year, book.isbn});

  qint64 id = _db->insert(query, &error);
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

  query.update(kTableName)
      .set({"title", "author", "year", "isbn"})
      .where("id = ?")
      .values({book.title, book.author, book.year, book.isbn, book.id});

  int affected = _db->execute(query, &error);

  if (affected > 0) {
    qCInfo(lcBookTable) << "Updated book id:" << book.id
                        << "title:" << book.title;
    return true;
  }

  if (affected == 0)
    qCWarning(lcBookTable) << "No book found with id:" << book.id;
  else
    qCWarning(lcBookTable) << "Failed to update book id:" << book.id
                           << "error:" << error;
  return false;
}

bool BookTable::deleteBook(qint64 id) {
  core::SqlQueryBuilder query;
  QString error;

  query.deleteFrom(kTableName).where("id = ?").values({id});

  int affected = _db->execute(query, &error);

  if (affected > 0) {
    qCInfo(lcBookTable) << "Deleted book id:" << id;
    return true;
  }

  if (affected == 0)
    qCWarning(lcBookTable) << "No book found with id:" << id;
  else
    qCWarning(lcBookTable) << "Failed to delete book id:" << id
                           << "error:" << error;
  return false;
}

bool BookTable::isbnExists(const QString &isbn, qint64 excludeId) {
  core::SqlQueryBuilder query;
  QString error;

  query.selectCount()
      .from(kTableName)
      .where("isbn = ? AND id != ?")
      .values({isbn, excludeId});

  auto data = _db->select(query, &error);

  if (data.isEmpty())
    return false;

  return data.first().value("COUNT(*)").toInt() > 0;
}

} // namespace bl::services
