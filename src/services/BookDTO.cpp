#include "BookDTO.hpp"

namespace bl::services {

QVariantMap BookDTO::toMap() const {
  return {
      {"id", id},     {"title", title}, {"author", author},
      {"year", year}, {"isbn", isbn},
  };
}

BookDTO BookDTO::fromMap(const QVariantMap &data) {
  BookDTO dto;
  dto.id = data.value("id").toLongLong();
  dto.title = data.value("title").toString();
  dto.author = data.value("author").toString();
  dto.year = data.value("year").toInt();
  dto.isbn = data.value("isbn").toString();
  return dto;
}

} // namespace bl::services
