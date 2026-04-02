#include "BookDTO.hpp"

#include <QTest>

using bl::services::BookDTO;

class tst_BookDTO : public QObject {
  Q_OBJECT

private slots:
  void toMapContainsAllFields() {
    BookDTO dto;
    dto.id = 42;
    dto.title = "Dune";
    dto.author = "Frank Herbert";
    dto.year = 1965;
    dto.isbn = "ISBN-12345";

    auto map = dto.toMap();

    QCOMPARE(map.value("id").toLongLong(), 42LL);
    QCOMPARE(map.value("title").toString(), "Dune");
    QCOMPARE(map.value("author").toString(), "Frank Herbert");
    QCOMPARE(map.value("year").toInt(), 1965);
    QCOMPARE(map.value("isbn").toString(), "ISBN-12345");
    QCOMPARE(map.size(), 5);
  }

  void fromMapParsesAllFields() {
    QVariantMap map = {{"id", 7},
                       {"title", "1984"},
                       {"author", "George Orwell"},
                       {"year", 1949},
                       {"isbn", "ISBN-999"}};

    auto dto = BookDTO::fromMap(map);

    QCOMPARE(dto.id, 7LL);
    QCOMPARE(dto.title, "1984");
    QCOMPARE(dto.author, "George Orwell");
    QCOMPARE(dto.year, 1949);
    QCOMPARE(dto.isbn, "ISBN-999");
  }

  void roundTrip() {
    BookDTO original;
    original.id = 1;
    original.title = "Test";
    original.author = "Author";
    original.year = 2000;
    original.isbn = "ISBN-111";

    auto restored = BookDTO::fromMap(original.toMap());

    QCOMPARE(restored.id, original.id);
    QCOMPARE(restored.title, original.title);
    QCOMPARE(restored.author, original.author);
    QCOMPARE(restored.year, original.year);
    QCOMPARE(restored.isbn, original.isbn);
  }

  void fromMapWithMissingFields() {
    QVariantMap map = {{"title", "Partial"}};

    auto dto = BookDTO::fromMap(map);

    QCOMPARE(dto.id, 0LL);
    QCOMPARE(dto.title, "Partial");
    QCOMPARE(dto.author, "");
    QCOMPARE(dto.year, 0);
    QCOMPARE(dto.isbn, "");
  }

  void defaultValues() {
    BookDTO dto;

    QCOMPARE(dto.id, 0LL);
    QCOMPARE(dto.title, "");
    QCOMPARE(dto.author, "");
    QCOMPARE(dto.year, 0);
    QCOMPARE(dto.isbn, "");
  }
};

QTEST_GUILESS_MAIN(tst_BookDTO)
#include "tst_BookDTO.moc"
