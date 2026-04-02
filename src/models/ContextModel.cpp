#include "ContextModel.hpp"

#include <QLoggingCategory>
#include <algorithm>

Q_LOGGING_CATEGORY(lcContext, "bl.models.context")

namespace bl::models {

QList<ContextModel::PageDTO> ContextModel::_pages = {
    {QUrl{"qrc:/qt/qml/Library/views/BookListPage.qml"},
     PageEnum::BOOK_LIST_PAGE, 1},
    {QUrl{"qrc:/qt/qml/Library/views/BookEditPage.qml"},
     PageEnum::BOOK_EDIT_PAGE, 2}};

ContextModel::ContextModel(QObject *parent) : QObject(parent) {
  setCurrentPage(PageEnum::BOOK_LIST_PAGE);
  qCInfo(lcContext) << "ContextModel initialized";
}

QUrl ContextModel::currentPagePath() const { return _pageStack.top()->url; }

ContextModel::PageEnum ContextModel::currentPage() const noexcept {
  return _pageStack.top()->page;
}

void ContextModel::setCurrentPage(PageEnum val) noexcept {
  auto newPage =
      std::find_if(_pages.begin(), _pages.end(),
                   [&val](const PageDTO &item) { return item.page == val; });

  if (_pageStack.empty()) {
    _pageStack.push(&(*newPage));
    qCInfo(lcContext) << "Page set:" << newPage->url << "(stack size: 1)";
    emit currentPageChanged();
    return;
  }

  PageDTO *current = _pageStack.top();

  if (current->page != val) {
    while (!_pageStack.empty() && newPage->level <= current->level) {
      _pageStack.pop();
    }
    _pageStack.push(&(*newPage));
    qCInfo(lcContext) << "Page changed:" << newPage->url
                      << "(stack size:" << _pageStack.size() << ")";
    emit currentPageChanged();
  }
}

void ContextModel::openPreviousPage() {
  if (!_pageStack.empty() && _pageStack.size() > 1) {
    _pageStack.pop();
    qCInfo(lcContext) << "Navigated back to:" << _pageStack.top()->url
                      << "(stack size:" << _pageStack.size() << ")";
    emit currentPageChanged();
  }
}

} // namespace bl::models
