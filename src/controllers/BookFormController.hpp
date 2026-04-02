#ifndef BEELIBRARY_CONTROLLERS_BOOKFORMCONTROLLER_HPP
#define BEELIBRARY_CONTROLLERS_BOOKFORMCONTROLLER_HPP

#include "services/BookTable.hpp"

#include <QObject>
#include <QVariantMap>
#include <memory>

namespace bl::models {
class BookListModel;
}

namespace bl::controllers {

class BookFormController : public QObject {
  Q_OBJECT

  Q_PROPERTY(int currentBookId READ currentBookId WRITE setCurrentBookId NOTIFY
                 currentBookIdChanged)
  Q_PROPERTY(QVariantMap currentBookData READ currentBookData NOTIFY
                 currentBookIdChanged)
  Q_PROPERTY(bool editMode READ editMode NOTIFY currentBookIdChanged)
  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
  Q_PROPERTY(int yearMin READ yearMin CONSTANT)
  Q_PROPERTY(int yearMax READ yearMax CONSTANT)

public:
  explicit BookFormController(std::shared_ptr<services::BookTable> bookTable,
                              bl::models::BookListModel *listModel,
                              QObject *parent = nullptr);

  Q_INVOKABLE bool submitForm(const QString &title, const QString &author,
                              const QString &yearStr, const QString &rawIsbn);
  Q_INVOKABLE bool isFormValid(const QString &title,
                               const QString &author) const;

  int currentBookId() const;
  void setCurrentBookId(int id);
  QVariantMap currentBookData() const;
  bool editMode() const;
  QString errorMessage() const;
  int yearMin() const;
  int yearMax() const;

signals:
  void currentBookIdChanged();
  void errorMessageChanged();
  void bookSaved();

private:
  static QString normalizeIsbn(const QString &rawIsbn);
  static QString stripIsbnPrefix(const QString &isbn);

  bool validate(const QString &title, const QString &author, int year,
                const QString &isbn);
  void setErrorMessage(const QString &message);

  std::shared_ptr<services::BookTable> _bookTable;
  bl::models::BookListModel *_listModel;
  QString _errorMessage;
  int _currentBookId = 0;
};

} // namespace bl::controllers

#endif // BEELIBRARY_CONTROLLERS_BOOKFORMCONTROLLER_HPP
