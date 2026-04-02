#include "BookProxyModel.hpp"

namespace bl::models {

BookProxyModel::BookProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

QString BookProxyModel::searchQuery() const { return _searchQuery; }

void BookProxyModel::setSearchQuery(const QString &query) {
  if (_searchQuery == query)
    return;

  _searchQuery = query;
  emit searchQueryChanged();
  invalidateFilter();
}

bool BookProxyModel::filterAcceptsRow(int sourceRow,
                                      const QModelIndex &sourceParent) const {
  if (_searchQuery.isEmpty())
    return true;

  QAbstractItemModel *model = sourceModel();
  QModelIndex idx = model->index(sourceRow, 0, sourceParent);

  QString title = model->data(idx, BookListModel::TitleRole).toString();
  QString author = model->data(idx, BookListModel::AuthorRole).toString();
  QString isbn = model->data(idx, BookListModel::IsbnRole).toString();

  return title.contains(_searchQuery, Qt::CaseInsensitive) ||
         author.contains(_searchQuery, Qt::CaseInsensitive) ||
         isbn.contains(_searchQuery, Qt::CaseInsensitive);
}

} // namespace bl::models
