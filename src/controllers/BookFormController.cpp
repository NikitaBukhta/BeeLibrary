#include "BookFormController.hpp"
#include "models/BookListModel.hpp"

#include <QDate>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcBookForm, "bl.controllers.bookform")

namespace bl::controllers {

static const QString kIsbnPrefix = QStringLiteral("ISBN-");

BookFormController::BookFormController(
    std::shared_ptr<services::BookTable> bookTable,
    bl::models::BookListModel *listModel, QObject *parent)
    : QObject(parent), _bookTable{std::move(bookTable)}, _listModel{listModel} {
}

bool BookFormController::submitForm(const QString &title, const QString &author,
                                    const QString &yearStr,
                                    const QString &rawIsbn) {
  QString trimmedYear = yearStr.trimmed();
  int year = trimmedYear.isEmpty() ? 0 : trimmedYear.toInt();
  QString isbn = normalizeIsbn(rawIsbn);

  if (!validate(title, author, year, isbn))
    return false;

  services::BookDTO book;
  book.title = title.trimmed();
  book.author = author.trimmed();
  book.year = year;
  book.isbn = isbn;

  if (_currentBookId > 0) {
    book.id = _currentBookId;
    if (!_bookTable->updateBook(book)) {
      setErrorMessage(tr("Failed to update book."));
      return false;
    }
    qCInfo(lcBookForm) << "Book updated — id:" << book.id
                       << "title:" << book.title;
  } else {
    qint64 id = _bookTable->addBook(book);
    if (id <= 0) {
      setErrorMessage(tr("Failed to add book."));
      return false;
    }
    qCInfo(lcBookForm) << "Book added — id:" << id << "title:" << book.title;
  }

  setErrorMessage({});
  emit bookSaved();
  return true;
}

bool BookFormController::isFormValid(const QString &title,
                                     const QString &author) const {
  return !title.trimmed().isEmpty() && !author.trimmed().isEmpty();
}

int BookFormController::currentBookId() const { return _currentBookId; }

void BookFormController::setCurrentBookId(int id) {
  if (_currentBookId == id)
    return;
  _currentBookId = id;
  emit currentBookIdChanged();
}

QVariantMap BookFormController::currentBookData() const {
  if (_currentBookId <= 0)
    return {};

  auto book = _listModel->getBook(_currentBookId);
  if (!book.isEmpty())
    book.insert("rawIsbn", stripIsbnPrefix(book.value("isbn").toString()));
  return book;
}

bool BookFormController::editMode() const { return _currentBookId > 0; }

QString BookFormController::errorMessage() const { return _errorMessage; }

int BookFormController::yearMin() const { return 1; }

int BookFormController::yearMax() const { return QDate::currentDate().year(); }

QString BookFormController::normalizeIsbn(const QString &rawIsbn) {
  QString trimmed = rawIsbn.trimmed();
  if (trimmed.isEmpty())
    return {};
  if (trimmed.startsWith(kIsbnPrefix))
    return trimmed;
  return kIsbnPrefix + trimmed;
}

QString BookFormController::stripIsbnPrefix(const QString &isbn) {
  if (isbn.startsWith(kIsbnPrefix))
    return isbn.mid(kIsbnPrefix.length());
  return isbn;
}

bool BookFormController::validate(const QString &title, const QString &author,
                                  int year, const QString &isbn) {
  if (title.trimmed().isEmpty()) {
    setErrorMessage(tr("Title is required."));
    return false;
  }

  if (author.trimmed().isEmpty()) {
    setErrorMessage(tr("Author is required."));
    return false;
  }

  if (year != 0 && (year < yearMin() || year > yearMax())) {
    setErrorMessage(
        tr("Year must be between %1 and %2.").arg(yearMin()).arg(yearMax()));
    return false;
  }

  if (!isbn.trimmed().isEmpty() &&
      _bookTable->isbnExists(isbn.trimmed(), _currentBookId)) {
    setErrorMessage(tr("A book with this ISBN already exists."));
    return false;
  }

  return true;
}

void BookFormController::setErrorMessage(const QString &message) {
  if (_errorMessage == message)
    return;
  _errorMessage = message;
  emit errorMessageChanged();
}

} // namespace bl::controllers
