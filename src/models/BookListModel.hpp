#ifndef BEELIBRARY_MODELS_BOOKLISTMODEL_HPP
#define BEELIBRARY_MODELS_BOOKLISTMODEL_HPP

#include "services/BookDTO.hpp"
#include "services/BookTable.hpp"

#include <QAbstractListModel>
#include <QList>
#include <memory>

namespace bl::models {

class BookListModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    TitleRole,
    AuthorRole,
    YearRole,
    IsbnRole,
  };
  Q_ENUM(Roles)

  explicit BookListModel(std::shared_ptr<services::BookTable> bookTable,
                         QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE bool deleteBook(int id);

  QVariantMap getBook(int id) const;
  void refresh();
  QString errorMessage() const;

signals:
  void errorMessageChanged();

private:
  void setErrorMessage(const QString &message);

  QList<services::BookDTO> _books;
  std::shared_ptr<services::BookTable> _bookTable;
  QString _errorMessage;
};

} // namespace bl::models

#endif // BEELIBRARY_MODELS_BOOKLISTMODEL_HPP
