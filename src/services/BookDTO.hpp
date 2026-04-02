#ifndef BEELIBRARY_SERVICES_BOOKDTO_HPP
#define BEELIBRARY_SERVICES_BOOKDTO_HPP

#include <QMap>
#include <QString>
#include <QVariant>
#include <QtTypes>

namespace bl::services {

struct BookDTO {
  qint64 id = 0;
  QString title;
  QString author;
  int year = 0;
  QString isbn;

  QVariantMap toMap() const;
  static BookDTO fromMap(const QVariantMap &data);
};

} // namespace bl::services

#endif // BEELIBRARY_SERVICES_BOOKDTO_HPP
