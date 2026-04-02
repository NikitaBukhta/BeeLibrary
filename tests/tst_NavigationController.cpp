#include "NavigationController.hpp"

#include <QSignalSpy>
#include <QTest>

using bl::controllers::NavigationController;
using PageEnum = NavigationController::PageEnum;

class tst_NavigationController : public QObject {
  Q_OBJECT

private slots:
  void initialState() {
    NavigationController nav;

    QCOMPARE(nav.currentPage(), PageEnum::BOOK_LIST_PAGE);
    QVERIFY(!nav.currentPagePath().isEmpty());
    QVERIFY(nav.currentPagePath().toString().contains("BookListPage"));
  }

  void navigateToNewPage() {
    NavigationController nav;
    QSignalSpy spy(&nav, &NavigationController::currentPageChanged);

    nav.setCurrentPage(PageEnum::BOOK_EDIT_PAGE);

    QCOMPARE(nav.currentPage(), PageEnum::BOOK_EDIT_PAGE);
    QVERIFY(nav.currentPagePath().toString().contains("BookEditPage"));
    QCOMPARE(spy.count(), 1);
  }

  void navigateToSamePageIsNoop() {
    NavigationController nav;
    QSignalSpy spy(&nav, &NavigationController::currentPageChanged);

    nav.setCurrentPage(PageEnum::BOOK_LIST_PAGE);

    QCOMPARE(spy.count(), 0);
  }

  void goBack() {
    NavigationController nav;
    nav.setCurrentPage(PageEnum::BOOK_EDIT_PAGE);

    QSignalSpy spy(&nav, &NavigationController::currentPageChanged);
    nav.goBack();

    QCOMPARE(nav.currentPage(), PageEnum::BOOK_LIST_PAGE);
    QCOMPARE(spy.count(), 1);
  }

  void goBackOnRootIsNoop() {
    NavigationController nav;
    QSignalSpy spy(&nav, &NavigationController::currentPageChanged);

    nav.goBack();

    QCOMPARE(nav.currentPage(), PageEnum::BOOK_LIST_PAGE);
    QCOMPARE(spy.count(), 0);
  }

  void navigateBackAndForward() {
    NavigationController nav;

    nav.setCurrentPage(PageEnum::BOOK_EDIT_PAGE);
    QCOMPARE(nav.currentPage(), PageEnum::BOOK_EDIT_PAGE);

    nav.goBack();
    QCOMPARE(nav.currentPage(), PageEnum::BOOK_LIST_PAGE);

    nav.setCurrentPage(PageEnum::BOOK_EDIT_PAGE);
    QCOMPARE(nav.currentPage(), PageEnum::BOOK_EDIT_PAGE);
  }

  void multipleGoBackStopsAtRoot() {
    NavigationController nav;
    nav.setCurrentPage(PageEnum::BOOK_EDIT_PAGE);

    nav.goBack();
    nav.goBack();
    nav.goBack();

    QCOMPARE(nav.currentPage(), PageEnum::BOOK_LIST_PAGE);
  }
};

QTEST_GUILESS_MAIN(tst_NavigationController)
#include "tst_NavigationController.moc"
