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

  // QAbstractListModel interface
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  // CRUD operations invokable from QML
  Q_INVOKABLE bool addBook(const QString &title, const QString &author,
                           int year, const QString &isbn);
  Q_INVOKABLE bool updateBook(int id, const QString &title,
                              const QString &author, int year,
                              const QString &isbn);
  Q_INVOKABLE bool deleteBook(int id);
  Q_INVOKABLE QVariantMap getBook(int id) const;

  // Error
  QString errorMessage() const;

signals:
  void errorMessageChanged();

private:
  void refresh();
  bool validate(const QString &title, const QString &author, int year,
                const QString &isbn, qint64 excludeId = 0);

  QList<services::BookDTO> _books;
  std::shared_ptr<services::BookTable> _bookTable;
  QString _errorMessage;
};

} // namespace bl::models

#endif // BEELIBRARY_MODELS_BOOKLISTMODEL_HPP
