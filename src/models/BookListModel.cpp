#include "BookListModel.hpp"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcBookModel, "bl.models.books")

namespace bl::models {

BookListModel::BookListModel(std::shared_ptr<services::BookTable> bookTable,
                             QObject *parent)
    : QAbstractListModel(parent), _bookTable{std::move(bookTable)} {
  refresh();
  qCInfo(lcBookModel) << "BookListModel initialized with" << _books.size()
                      << "books";
}

int BookListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return static_cast<int>(_books.size());
}

QVariant BookListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= _books.size())
    return {};

  const auto &book = _books.at(index.row());

  switch (role) {
  case IdRole:
    return book.id;
  case TitleRole:
    return book.title;
  case AuthorRole:
    return book.author;
  case YearRole:
    return book.year;
  case IsbnRole:
    return book.isbn;
  default:
    return {};
  }
}

QHash<int, QByteArray> BookListModel::roleNames() const {
  return {
      {IdRole, "bookId"}, {TitleRole, "title"}, {AuthorRole, "author"},
      {YearRole, "year"}, {IsbnRole, "isbn"},
  };
}

bool BookListModel::addBook(const QString &title, const QString &author,
                            int year, const QString &isbn) {
  if (!validate(title, author, year, isbn))
    return false;

  services::BookDTO book;
  book.title = title.trimmed();
  book.author = author.trimmed();
  book.year = year;
  book.isbn = isbn.trimmed();

  qint64 id = _bookTable->addBook(book);
  if (id <= 0) {
    _errorMessage = tr("Failed to add book.");
    emit errorMessageChanged();
    return false;
  }

  qCInfo(lcBookModel) << "Book added — id:" << id << "title:" << book.title;
  _errorMessage.clear();
  emit errorMessageChanged();
  refresh();
  return true;
}

bool BookListModel::updateBook(int id, const QString &title,
                               const QString &author, int year,
                               const QString &isbn) {
  if (!validate(title, author, year, isbn, id))
    return false;

  services::BookDTO book;
  book.id = id;
  book.title = title.trimmed();
  book.author = author.trimmed();
  book.year = year;
  book.isbn = isbn.trimmed();

  if (!_bookTable->updateBook(book)) {
    _errorMessage = tr("Failed to update book.");
    emit errorMessageChanged();
    return false;
  }

  qCInfo(lcBookModel) << "Book updated — id:" << id << "title:" << book.title;
  _errorMessage.clear();
  emit errorMessageChanged();
  refresh();
  return true;
}

bool BookListModel::deleteBook(int id) {
  if (!_bookTable->deleteBook(id)) {
    _errorMessage = tr("Failed to delete book.");
    emit errorMessageChanged();
    return false;
  }

  qCInfo(lcBookModel) << "Book deleted — id:" << id;
  _errorMessage.clear();
  emit errorMessageChanged();
  refresh();
  return true;
}

QVariantMap BookListModel::getBook(int id) const {
  for (const auto &book : _books) {
    if (book.id == id) {
      qCDebug(lcBookModel) << "getBook found id:" << id
                           << "title:" << book.title;
      return {
          {"id", book.id},     {"title", book.title}, {"author", book.author},
          {"year", book.year}, {"isbn", book.isbn},
      };
    }
  }
  qCWarning(lcBookModel) << "getBook — book not found, id:" << id;
  return {};
}

QString BookListModel::errorMessage() const { return _errorMessage; }

void BookListModel::refresh() {
  beginResetModel();
  _bookTable->getAllBooks(_books);
  endResetModel();
}

bool BookListModel::validate(const QString &title, const QString &author,
                             int year, const QString &isbn, qint64 excludeId) {
  if (title.trimmed().isEmpty()) {
    qCWarning(lcBookModel) << "Validation failed: title is empty";
    _errorMessage = tr("Title is required.");
    emit errorMessageChanged();
    return false;
  }

  if (author.trimmed().isEmpty()) {
    qCWarning(lcBookModel) << "Validation failed: author is empty";
    _errorMessage = tr("Author is required.");
    emit errorMessageChanged();
    return false;
  }

  if (year != 0 && (year < 1 || year > 9999)) {
    qCWarning(lcBookModel) << "Validation failed: invalid year" << year;
    _errorMessage = tr("Year must be between 1 and 9999.");
    emit errorMessageChanged();
    return false;
  }

  if (!isbn.trimmed().isEmpty() &&
      _bookTable->isbnExists(isbn.trimmed(), excludeId)) {
    qCWarning(lcBookModel) << "Validation failed: ISBN already exists:" << isbn;
    _errorMessage = tr("A book with this ISBN already exists.");
    emit errorMessageChanged();
    return false;
  }

  return true;
}

} // namespace bl::models
