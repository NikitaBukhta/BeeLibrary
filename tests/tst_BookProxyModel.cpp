#include "BookListModel.hpp"
#include "BookProxyModel.hpp"
#include "BookTable.hpp"
#include "DatabaseManager.hpp"

#include <QSignalSpy>
#include <QTest>

using bl::core::DatabaseManager;
using bl::models::BookListModel;
using bl::models::BookProxyModel;
using bl::services::BookDTO;
using bl::services::BookTable;

class tst_BookProxyModel : public QObject {
  Q_OBJECT

private:
  std::shared_ptr<DatabaseManager> _db;
  std::shared_ptr<BookTable> _bookTable;
  BookListModel *_source = nullptr;
  BookProxyModel *_proxy = nullptr;

  void createSchema() {
    QString error;
    _db->exec("CREATE TABLE IF NOT EXISTS books ("
              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
              "title TEXT NOT NULL, "
              "author TEXT NOT NULL, "
              "year INTEGER DEFAULT NULL, "
              "isbn TEXT DEFAULT NULL);",
              &error);
  }

  void addBook(const QString &title, const QString &author, int year = 0,
               const QString &isbn = {}) {
    BookDTO book;
    book.title = title;
    book.author = author;
    book.year = year;
    book.isbn = isbn;
    _bookTable->addBook(book);
  }

  void addTestBooks() {
    addBook("Dune", "Frank Herbert", 1965, "ISBN-001");
    addBook("1984", "George Orwell", 1949, "ISBN-002");
    addBook("Neuromancer", "William Gibson", 1984, "ISBN-003");
    _source->refresh();
  }

  QString proxyTitle(int row) const {
    return _proxy->data(_proxy->index(row, 0), BookListModel::TitleRole)
        .toString();
  }

  int proxyYear(int row) const {
    return _proxy->data(_proxy->index(row, 0), BookListModel::YearRole).toInt();
  }

private slots:
  void init() {
    _db = std::make_shared<DatabaseManager>(":memory:");
    _db->open();
    createSchema();

    _bookTable = std::make_shared<BookTable>(_db);
    _source = new BookListModel(_bookTable, this);
    _proxy = new BookProxyModel(this);
    _proxy->setSourceModel(_source);
  }

  void cleanup() {
    delete _proxy;
    _proxy = nullptr;
    delete _source;
    _source = nullptr;
    _bookTable.reset();
    _db.reset();
  }

  void defaultSortByTitle() {
    addTestBooks();

    QCOMPARE(proxyTitle(0), "1984");
    QCOMPARE(proxyTitle(1), "Dune");
    QCOMPARE(proxyTitle(2), "Neuromancer");
  }

  void sortByAuthor() {
    addTestBooks();

    _proxy->setSortField(BookListModel::AuthorRole);

    QCOMPARE(proxyTitle(0), "Dune");
    QCOMPARE(proxyTitle(1), "1984");
    QCOMPARE(proxyTitle(2), "Neuromancer");
  }

  void sortByYear() {
    addTestBooks();

    _proxy->setSortField(BookListModel::YearRole);

    QCOMPARE(proxyYear(0), 1949);
    QCOMPARE(proxyYear(1), 1965);
    QCOMPARE(proxyYear(2), 1984);
  }

  void sortDescending() {
    addTestBooks();

    _proxy->setSortDescending(true);

    QCOMPARE(proxyTitle(0), "Neuromancer");
    QCOMPARE(proxyTitle(1), "Dune");
    QCOMPARE(proxyTitle(2), "1984");
  }

  void sortByYearDescending() {
    addTestBooks();

    _proxy->setSortField(BookListModel::YearRole);
    _proxy->setSortDescending(true);

    QCOMPARE(proxyYear(0), 1984);
    QCOMPARE(proxyYear(1), 1965);
    QCOMPARE(proxyYear(2), 1949);
  }

  void sortDescendingDefaultFalse() {
    QVERIFY(!_proxy->sortDescending());
  }

  void setSortDescendingEmitsSignal() {
    QSignalSpy spy(_proxy, &BookProxyModel::sortDescendingChanged);

    _proxy->setSortDescending(true);

    QCOMPARE(spy.count(), 1);
    QVERIFY(_proxy->sortDescending());
  }

  void setSortDescendingSameValueNoSignal() {
    QSignalSpy spy(_proxy, &BookProxyModel::sortDescendingChanged);

    _proxy->setSortDescending(false);

    QCOMPARE(spy.count(), 0);
  }

  void setSortFieldEmitsSignal() {
    QSignalSpy spy(_proxy, &BookProxyModel::sortFieldChanged);

    _proxy->setSortField(BookListModel::AuthorRole);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(_proxy->sortRole(), static_cast<int>(BookListModel::AuthorRole));
  }

  void setSortFieldSameValueNoSignal() {
    QSignalSpy spy(_proxy, &BookProxyModel::sortFieldChanged);

    _proxy->setSortField(BookListModel::TitleRole);

    QCOMPARE(spy.count(), 0);
  }

  void filterByTitle() {
    addTestBooks();

    _proxy->setSearchQuery("dune");

    QCOMPARE(_proxy->rowCount(), 1);
    QCOMPARE(proxyTitle(0), "Dune");
  }

  void filterByAuthor() {
    addTestBooks();

    _proxy->setSearchQuery("orwell");

    QCOMPARE(_proxy->rowCount(), 1);
    QCOMPARE(proxyTitle(0), "1984");
  }

  void filterByIsbn() {
    addTestBooks();

    _proxy->setSearchQuery("ISBN-003");

    QCOMPARE(_proxy->rowCount(), 1);
    QCOMPARE(proxyTitle(0), "Neuromancer");
  }

  void filterCaseInsensitive() {
    addTestBooks();

    _proxy->setSearchQuery("DUNE");

    QCOMPARE(_proxy->rowCount(), 1);
  }

  void filterEmptyShowsAll() {
    addTestBooks();

    _proxy->setSearchQuery("dune");
    QCOMPARE(_proxy->rowCount(), 1);

    _proxy->setSearchQuery("");
    QCOMPARE(_proxy->rowCount(), 3);
  }

  void filterNoMatch() {
    addTestBooks();

    _proxy->setSearchQuery("nonexistent");

    QCOMPARE(_proxy->rowCount(), 0);
  }

  void filterAndSortCombined() {
    addBook("Alpha Book", "Zeta Author", 2000);
    addBook("Alpha Novel", "Alpha Author", 1990);
    addBook("Beta Book", "Gamma Author", 2010);
    _source->refresh();

    _proxy->setSearchQuery("alpha");
    _proxy->setSortField(BookListModel::YearRole);

    QCOMPARE(_proxy->rowCount(), 2);
    QCOMPARE(proxyYear(0), 1990);
    QCOMPARE(proxyYear(1), 2000);
  }

  void setSearchQueryEmitsSignal() {
    QSignalSpy spy(_proxy, &BookProxyModel::searchQueryChanged);

    _proxy->setSearchQuery("test");

    QCOMPARE(spy.count(), 1);
    QCOMPARE(_proxy->searchQuery(), "test");
  }

  void setSearchQuerySameValueNoSignal() {
    _proxy->setSearchQuery("test");
    QSignalSpy spy(_proxy, &BookProxyModel::searchQueryChanged);

    _proxy->setSearchQuery("test");

    QCOMPARE(spy.count(), 0);
  }
};

QTEST_GUILESS_MAIN(tst_BookProxyModel)
#include "tst_BookProxyModel.moc"
