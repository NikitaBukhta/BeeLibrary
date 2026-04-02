#ifndef BEELIBRARY_CONTROLLERS_CONTEXTMODEL_HPP
#define BEELIBRARY_CONTROLLERS_CONTEXTMODEL_HPP

#include <QObject>
#include <QStack>
#include <QUrl>

namespace bl::controllers {

class ContextModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(
      QUrl currentPagePath READ currentPagePath NOTIFY currentPageChanged FINAL)
  Q_PROPERTY(
      PageEnum currentPage READ currentPage NOTIFY currentPageChanged FINAL)

public:
  enum class PageEnum { BOOK_LIST_PAGE = 1, BOOK_EDIT_PAGE };
  Q_ENUM(PageEnum)

  explicit ContextModel(QObject *parent = nullptr);

  QUrl currentPagePath() const;
  PageEnum currentPage() const;

  Q_INVOKABLE void navigateTo(PageEnum page);
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

#endif // BEELIBRARY_CONTROLLERS_CONTEXTMODEL_HPP
