#include "BookDTO.hpp"

namespace bl::services {

BookDTO BookDTOMapper::map(const QMap<QString, QVariant> &data) {
  BookDTO dto;
  dto.id = data["id"].toLongLong();
  dto.title = data["title"].toString();
  dto.author = data["author"].toString();
  dto.year = data["year"].toInt();
  dto.isbn = data["isbn"].toString();
  return dto;
}

} // namespace bl::services
