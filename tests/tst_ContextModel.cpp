#include "ContextModel.hpp"

#include <QSignalSpy>
#include <QTest>

using bl::controllers::ContextModel;
using PageEnum = ContextModel::PageEnum;

class tst_ContextModel : public QObject {
  Q_OBJECT

private slots:
  void initialState() {
    ContextModel model;

    QCOMPARE(model.currentPage(), PageEnum::BOOK_LIST_PAGE);
    QVERIFY(!model.currentPagePath().isEmpty());
    QVERIFY(model.currentPagePath().toString().contains("BookListPage"));
  }

  void navigateToNewPage() {
    ContextModel model;
    QSignalSpy spy(&model, &ContextModel::currentPageChanged);

    model.navigateTo(PageEnum::BOOK_EDIT_PAGE);

    QCOMPARE(model.currentPage(), PageEnum::BOOK_EDIT_PAGE);
    QVERIFY(model.currentPagePath().toString().contains("BookEditPage"));
    QCOMPARE(spy.count(), 1);
  }

  void navigateToSamePageIsNoop() {
    ContextModel model;
    QSignalSpy spy(&model, &ContextModel::currentPageChanged);

    model.navigateTo(PageEnum::BOOK_LIST_PAGE);

    QCOMPARE(spy.count(), 0);
  }

  void goBack() {
    ContextModel model;
    model.navigateTo(PageEnum::BOOK_EDIT_PAGE);

    QSignalSpy spy(&model, &ContextModel::currentPageChanged);
    model.goBack();

    QCOMPARE(model.currentPage(), PageEnum::BOOK_LIST_PAGE);
    QCOMPARE(spy.count(), 1);
  }

  void goBackOnRootIsNoop() {
    ContextModel model;
    QSignalSpy spy(&model, &ContextModel::currentPageChanged);

    model.goBack();

    QCOMPARE(model.currentPage(), PageEnum::BOOK_LIST_PAGE);
    QCOMPARE(spy.count(), 0);
  }

  void navigateBackAndForward() {
    ContextModel model;

    model.navigateTo(PageEnum::BOOK_EDIT_PAGE);
    QCOMPARE(model.currentPage(), PageEnum::BOOK_EDIT_PAGE);

    model.goBack();
    QCOMPARE(model.currentPage(), PageEnum::BOOK_LIST_PAGE);

    model.navigateTo(PageEnum::BOOK_EDIT_PAGE);
    QCOMPARE(model.currentPage(), PageEnum::BOOK_EDIT_PAGE);
  }

  void multipleGoBackStopsAtRoot() {
    ContextModel model;
    model.navigateTo(PageEnum::BOOK_EDIT_PAGE);

    model.goBack();
    model.goBack();
    model.goBack();

    QCOMPARE(model.currentPage(), PageEnum::BOOK_LIST_PAGE);
  }
};

QTEST_GUILESS_MAIN(tst_ContextModel)
#include "tst_ContextModel.moc"
