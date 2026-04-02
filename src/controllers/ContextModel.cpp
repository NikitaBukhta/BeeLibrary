#include "ContextModel.hpp"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcContext, "bl.controllers.context")

namespace bl::controllers {

ContextModel::ContextModel(QObject *parent) : QObject(parent) {
  navigateTo(PageEnum::BOOK_LIST_PAGE);
  qCInfo(lcContext) << "ContextModel initialized";
}

ContextModel::PageInfo ContextModel::pageInfo(PageEnum page) {
  switch (page) {
  case PageEnum::BOOK_LIST_PAGE:
    return {QUrl{u"qrc:/qt/qml/Library/pages/booklist/BookListPage.qml"_qs}, 1};
  case PageEnum::BOOK_EDIT_PAGE:
    return {QUrl{u"qrc:/qt/qml/Library/pages/bookedit/BookEditPage.qml"_qs}, 2};
  }
  Q_UNREACHABLE();
}

QUrl ContextModel::currentPagePath() const {
  if (_pageStack.empty())
    return {};
  return pageInfo(_pageStack.top()).url;
}

ContextModel::PageEnum ContextModel::currentPage() const {
  if (_pageStack.empty())
    return PageEnum::BOOK_LIST_PAGE;
  return _pageStack.top();
}

void ContextModel::navigateTo(PageEnum page) {
  if (!_pageStack.empty() && _pageStack.top() == page)
    return;

  const auto target = pageInfo(page);

  while (!_pageStack.empty() &&
         target.level <= pageInfo(_pageStack.top()).level) {
    _pageStack.pop();
  }

  _pageStack.push(page);
  qCInfo(lcContext) << "Page changed:" << target.url
                    << "(stack size:" << _pageStack.size() << ")";
  emit currentPageChanged();
}

void ContextModel::goBack() {
  if (_pageStack.size() <= 1)
    return;

  _pageStack.pop();
  qCInfo(lcContext) << "Navigated back to:" << pageInfo(_pageStack.top()).url
                    << "(stack size:" << _pageStack.size() << ")";
  emit currentPageChanged();
}

} // namespace bl::controllers
