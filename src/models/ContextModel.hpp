#ifndef BEELIBRARY_MODELS_CONTEXTMODEL_HPP
#define BEELIBRARY_MODELS_CONTEXTMODEL_HPP

#include <QList>
#include <QObject>
#include <QStack>
#include <QUrl>
#include <QtTypes>

namespace bl::models {

class ContextModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QUrl currentPagePath READ currentPagePath NOTIFY currentPageChanged FINAL)
  Q_PROPERTY(PageEnum currentPage READ currentPage WRITE setCurrentPage NOTIFY
                 currentPageChanged FINAL)

public:
  enum class PageEnum { BOOK_LIST_PAGE = 1, BOOK_EDIT_PAGE };
  Q_ENUM(PageEnum)

  explicit ContextModel(QObject *parent = nullptr);

  QUrl currentPagePath() const;
  PageEnum currentPage() const noexcept;
  void setCurrentPage(PageEnum val) noexcept;

  Q_INVOKABLE void openPreviousPage();

signals:
  void currentPageChanged();

private:
  struct PageDTO {
    QUrl url;
    PageEnum page;
    qint8 level;
  };

  static QList<PageDTO> _pages;
  QStack<PageDTO *> _pageStack;
};

} // namespace bl::models

#endif // BEELIBRARY_MODELS_CONTEXTMODEL_HPP
