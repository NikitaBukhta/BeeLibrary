#include "NavigationController.hpp"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcNavigation, "bl.controllers.navigation")

namespace bl::controllers {

NavigationController::NavigationController(QObject *parent) : QObject(parent) {
  setCurrentPage(PageEnum::BOOK_LIST_PAGE);
  qCInfo(lcNavigation) << "NavigationController initialized";
}

NavigationController::PageInfo NavigationController::pageInfo(PageEnum page) {
  switch (page) {
  case PageEnum::BOOK_LIST_PAGE:
    return {QUrl{u"qrc:/qt/qml/Library/pages/booklist/BookListPage.qml"_qs}, 1};
  case PageEnum::BOOK_EDIT_PAGE:
    return {QUrl{u"qrc:/qt/qml/Library/pages/bookedit/BookEditPage.qml"_qs}, 2};
  }
  Q_UNREACHABLE();
}

QUrl NavigationController::currentPagePath() const {
  if (_pageStack.empty())
    return {};
  return pageInfo(_pageStack.top()).url;
}

NavigationController::PageEnum NavigationController::currentPage() const {
  if (_pageStack.empty())
    return PageEnum::BOOK_LIST_PAGE;
  return _pageStack.top();
}

void NavigationController::setCurrentPage(PageEnum page) {
  if (!_pageStack.empty() && _pageStack.top() == page)
    return;

  const auto target = pageInfo(page);

  while (!_pageStack.empty() &&
         target.level <= pageInfo(_pageStack.top()).level) {
    _pageStack.pop();
  }

  _pageStack.push(page);
  qCInfo(lcNavigation) << "Page changed:" << target.url
                       << "(stack size:" << _pageStack.size() << ")";
  emit currentPageChanged();
}

void NavigationController::goBack() {
  if (_pageStack.size() <= 1)
    return;

  _pageStack.pop();
  qCInfo(lcNavigation) << "Navigated back to:" << pageInfo(_pageStack.top()).url
                       << "(stack size:" << _pageStack.size() << ")";
  emit currentPageChanged();
}

} // namespace bl::controllers
