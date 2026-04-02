#ifndef BEELIBRARY_SERVICES_BOOKTABLE_HPP
#define BEELIBRARY_SERVICES_BOOKTABLE_HPP

#include "BookDTO.hpp"
#include "core/DatabaseManager.hpp"

#include <QString>
#include <memory>

namespace bl::services {

class BookTable {
public:
  BookTable(std::weak_ptr<core::DatabaseManager> db);

  void getAllBooks(QList<BookDTO> &out);
  qint64 addBook(const BookDTO &book);
  bool updateBook(const BookDTO &book);
  bool deleteBook(qint64 id);
  bool isbnExists(const QString &isbn, qint64 excludeId = 0);

private:
  static const QString _tableName;
  std::shared_ptr<core::DatabaseManager> _db;
};

} // namespace bl::services

#endif // BEELIBRARY_SERVICES_BOOKTABLE_HPP
