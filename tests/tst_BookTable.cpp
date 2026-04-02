#include "BookTable.hpp"
#include "DatabaseManager.hpp"

#include <QTest>

using bl::core::DatabaseManager;
using bl::services::BookDTO;
using bl::services::BookTable;

class tst_BookTable : public QObject {
  Q_OBJECT

private:
  std::shared_ptr<DatabaseManager> _db;
  std::unique_ptr<BookTable> _table;

  void createSchema() {
    QString error;
    _db->exec("CREATE TABLE IF NOT EXISTS books ("
              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
              "title TEXT NOT NULL, "
              "author TEXT NOT NULL, "
              "year INTEGER DEFAULT NULL, "
              "isbn TEXT DEFAULT NULL);",
              &error);
    _db->exec(
        "CREATE UNIQUE INDEX IF NOT EXISTS ux_books_isbn ON books(isbn);",
        &error);
  }

  BookDTO makeBook(const QString &title, const QString &author, int year = 0,
                   const QString &isbn = {}) {
    BookDTO book;
    book.title = title;
    book.author = author;
    book.year = year;
    book.isbn = isbn;
    return book;
  }

private slots:
  void init() {
    _db = std::make_shared<DatabaseManager>(":memory:");
    _db->open();
    createSchema();
    _table = std::make_unique<BookTable>(_db);
  }

  void cleanup() {
    _table.reset();
    _db.reset();
  }

  void addBookReturnsId() {
    qint64 id = _table->addBook(makeBook("Dune", "Herbert", 1965));
    QVERIFY(id > 0);
  }

  void getAllBooksReturnsInserted() {
    _table->addBook(makeBook("Book A", "Author A"));
    _table->addBook(makeBook("Book B", "Author B"));

    auto books = _table->getAllBooks();
    QCOMPARE(books.size(), 2);
  }

  void getAllBooksOrderedByTitle() {
    _table->addBook(makeBook("Zebra", "Z"));
    _table->addBook(makeBook("Apple", "A"));

    auto books = _table->getAllBooks();
    QCOMPARE(books.at(0).title, "Apple");
    QCOMPARE(books.at(1).title, "Zebra");
  }

  void updateBook() {
    qint64 id = _table->addBook(makeBook("Old Title", "Author"));

    BookDTO updated;
    updated.id = id;
    updated.title = "New Title";
    updated.author = "Author";

    QVERIFY(_table->updateBook(updated));

    auto books = _table->getAllBooks();
    QCOMPARE(books.at(0).title, "New Title");
  }

  void updateNonexistentReturnsFalse() {
    BookDTO book;
    book.id = 99999;
    book.title = "Ghost";
    book.author = "Nobody";

    QVERIFY(!_table->updateBook(book));
  }

  void deleteBook() {
    qint64 id = _table->addBook(makeBook("To Delete", "Author"));
    QCOMPARE(_table->getAllBooks().size(), 1);

    QVERIFY(_table->deleteBook(id));
    QCOMPARE(_table->getAllBooks().size(), 0);
  }

  void deleteNonexistentReturnsFalse() {
    QVERIFY(!_table->deleteBook(99999));
  }

  void isbnExistsFindsMatch() {
    _table->addBook(makeBook("Book", "Author", 0, "ISBN-123"));

    QVERIFY(_table->isbnExists("ISBN-123"));
    QVERIFY(!_table->isbnExists("ISBN-999"));
  }

  void isbnExistsExcludesId() {
    qint64 id = _table->addBook(makeBook("Book", "Author", 0, "ISBN-123"));

    QVERIFY(!_table->isbnExists("ISBN-123", id));
    QVERIFY(_table->isbnExists("ISBN-123", id + 1));
  }
};

QTEST_GUILESS_MAIN(tst_BookTable)
#include "tst_BookTable.moc"
