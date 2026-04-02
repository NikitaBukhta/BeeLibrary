#ifndef BEELIBRARY_MODELS_BOOKPROXYMODEL_HPP
#define BEELIBRARY_MODELS_BOOKPROXYMODEL_HPP

#include "BookListModel.hpp"

#include <QSortFilterProxyModel>
#include <QString>

namespace bl::models {

class BookProxyModel : public QSortFilterProxyModel {
  Q_OBJECT

  Q_PROPERTY(QString searchQuery READ searchQuery WRITE setSearchQuery NOTIFY
                 searchQueryChanged)

public:
  explicit BookProxyModel(QObject *parent = nullptr);

  QString searchQuery() const;
  void setSearchQuery(const QString &query);

signals:
  void searchQueryChanged();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private:
  QString _searchQuery;
};

} // namespace bl::models

#endif // BEELIBRARY_MODELS_BOOKPROXYMODEL_HPP
