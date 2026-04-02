#ifndef BEELIBRARY_CONTROLLERS_NAVIGATIONCONTROLLER_HPP
#define BEELIBRARY_CONTROLLERS_NAVIGATIONCONTROLLER_HPP

#include <QObject>
#include <QStack>
#include <QUrl>

namespace bl::controllers {

class NavigationController : public QObject {
  Q_OBJECT

  Q_PROPERTY(
      QUrl currentPagePath READ currentPagePath NOTIFY currentPageChanged FINAL)
  Q_PROPERTY(PageEnum currentPage READ currentPage WRITE setCurrentPage NOTIFY
                 currentPageChanged FINAL)

public:
  enum class PageEnum { BOOK_LIST_PAGE = 1, BOOK_EDIT_PAGE };
  Q_ENUM(PageEnum)

  explicit NavigationController(QObject *parent = nullptr);

  QUrl currentPagePath() const;
  PageEnum currentPage() const;
  void setCurrentPage(PageEnum page);

  Q_INVOKABLE void goBack();

signals:
  void currentPageChanged();

private:
  struct PageInfo {
    QUrl url;
    qint8 level;
  };

  static PageInfo pageInfo(PageEnum page);

  QStack<PageEnum> _pageStack;
};

} // namespace bl::controllers

#endif // BEELIBRARY_CONTROLLERS_NAVIGATIONCONTROLLER_HPP
