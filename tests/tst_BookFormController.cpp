#include "BookFormController.hpp"
#include "BookListModel.hpp"
#include "BookTable.hpp"
#include "DatabaseManager.hpp"

#include <QSignalSpy>
#include <QTest>

using bl::controllers::BookFormController;
using bl::core::DatabaseManager;
using bl::models::BookListModel;
using bl::services::BookTable;

class tst_BookFormController : public QObject {
  Q_OBJECT

private:
  std::shared_ptr<DatabaseManager> _db;
  std::shared_ptr<BookTable> _bookTable;
  BookListModel *_listModel = nullptr;
  BookFormController *_controller = nullptr;

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

private slots:
  void init() {
    _db = std::make_shared<DatabaseManager>(":memory:");
    _db->open();
    createSchema();

    _bookTable = std::make_shared<BookTable>(_db);
    _listModel = new BookListModel(_bookTable, this);
    _controller = new BookFormController(_bookTable, _listModel, this);
  }

  void cleanup() {
    delete _controller;
    _controller = nullptr;
    delete _listModel;
    _listModel = nullptr;
    _bookTable.reset();
    _db.reset();
  }

  void isFormValidRequiresTitleAndAuthor() {
    QVERIFY(!_controller->isFormValid("", ""));
    QVERIFY(!_controller->isFormValid("Title", ""));
    QVERIFY(!_controller->isFormValid("", "Author"));
    QVERIFY(!_controller->isFormValid("  ", "  "));
    QVERIFY(_controller->isFormValid("Title", "Author"));
  }

  void editModeDefaultFalse() {
    QVERIFY(!_controller->editMode());
    QCOMPARE(_controller->currentBookId(), 0);
  }

  void setCurrentBookIdEmitsSignal() {
    QSignalSpy spy(_controller, &BookFormController::currentBookIdChanged);

    _controller->setCurrentBookId(5);

    QCOMPARE(_controller->currentBookId(), 5);
    QVERIFY(_controller->editMode());
    QCOMPARE(spy.count(), 1);
  }

  void setCurrentBookIdSameValueNoSignal() {
    _controller->setCurrentBookId(3);
    QSignalSpy spy(_controller, &BookFormController::currentBookIdChanged);

    _controller->setCurrentBookId(3);

    QCOMPARE(spy.count(), 0);
  }

  void submitFormAddsBook() {
    QSignalSpy spy(_controller, &BookFormController::bookSaved);

    bool ok = _controller->submitForm("Dune", "Herbert", "1965", "123");

    QVERIFY(ok);
    QCOMPARE(spy.count(), 1);
  }

  void submitFormUpdatesBook() {
    _controller->submitForm("Old", "Author", "", "");
    _listModel->refresh();

    auto books = _bookTable->getAllBooks();
    QVERIFY(!books.isEmpty());

    _controller->setCurrentBookId(books.first().id);
    bool ok = _controller->submitForm("New", "Author", "", "");

    QVERIFY(ok);
    auto updated = _bookTable->getAllBooks();
    QCOMPARE(updated.first().title, "New");
  }

  void submitFormValidationFails() {
    bool ok = _controller->submitForm("", "", "", "");

    QVERIFY(!ok);
    QVERIFY(!_controller->errorMessage().isEmpty());
  }

  void submitFormClearsErrorOnSuccess() {
    _controller->submitForm("", "", "", "");
    QVERIFY(!_controller->errorMessage().isEmpty());

    _controller->submitForm("Title", "Author", "", "");
    QCOMPARE(_controller->errorMessage(), "");
  }

  void submitFormRejectsInvalidYear() {
    bool ok = _controller->submitForm("Title", "Author", "99999", "");

    QVERIFY(!ok);
    QVERIFY(_controller->errorMessage().contains("Year"));
  }

  void submitFormRejectsDuplicateIsbn() {
    _controller->submitForm("Book1", "Author", "", "123");

    _controller->setCurrentBookId(0);
    bool ok = _controller->submitForm("Book2", "Author", "", "123");

    QVERIFY(!ok);
    QVERIFY(_controller->errorMessage().contains("ISBN"));
  }

  void yearMinMax() {
    QCOMPARE(_controller->yearMin(), 1);
    QVERIFY(_controller->yearMax() >= 2024);
  }

  void currentBookDataForNewBook() {
    _controller->setCurrentBookId(0);
    QVERIFY(_controller->currentBookData().isEmpty());
  }

  void currentBookDataForExistingBook() {
    _controller->submitForm("Dune", "Herbert", "1965", "");
    _listModel->refresh();

    auto books = _bookTable->getAllBooks();
    QVERIFY(!books.isEmpty());
    _controller->setCurrentBookId(books.first().id);

    auto data = _controller->currentBookData();
    QCOMPARE(data.value("title").toString(), "Dune");
    QCOMPARE(data.value("author").toString(), "Herbert");
  }
};

QTEST_GUILESS_MAIN(tst_BookFormController)
#include "tst_BookFormController.moc"
