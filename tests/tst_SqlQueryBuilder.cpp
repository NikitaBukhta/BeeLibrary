#include "SqlQueryBuilder.hpp"

#include <QTest>

using bl::core::SqlQueryBuilder;

class tst_SqlQueryBuilder : public QObject {
  Q_OBJECT

private slots:
  void selectAll() {
    SqlQueryBuilder qb;
    qb.select().from("books");
    QCOMPARE(qb.build(), "SELECT * FROM books");
  }

  void selectColumns() {
    SqlQueryBuilder qb;
    qb.select({"id", "title"}).from("books");
    QCOMPARE(qb.build(), "SELECT id, title FROM books");
  }

  void selectCount() {
    SqlQueryBuilder qb;
    qb.selectCount().from("books");
    QCOMPARE(qb.build(), "SELECT COUNT(*) FROM books");
  }

  void selectWithWhere() {
    SqlQueryBuilder qb;
    qb.select({"id"}).from("books").where("id = ?").values({42});
    QCOMPARE(qb.build(), "SELECT id FROM books WHERE id = ?");
    QCOMPARE(qb.getValues(), QVariantList{42});
  }

  void selectWithOrderBy() {
    SqlQueryBuilder qb;
    qb.select({"title"}).from("books").orderBy("title");
    QCOMPARE(qb.build(), "SELECT title FROM books ORDER BY title ASC");
  }

  void selectWithOrderByDesc() {
    SqlQueryBuilder qb;
    qb.select({"title"}).from("books").orderBy("year", "DESC");
    QCOMPARE(qb.build(), "SELECT title FROM books ORDER BY year DESC");
  }

  void insertInto() {
    SqlQueryBuilder qb;
    qb.insertInto("books", {"title", "author"}).values({"Dune", "Herbert"});
    QCOMPARE(qb.build(), "INSERT INTO books (title, author) VALUES (?, ?)");
    QCOMPARE(qb.getValues(), QVariantList({"Dune", "Herbert"}));
  }

  void update() {
    SqlQueryBuilder qb;
    qb.update("books").set({"title"}).where("id = ?").values({"New Title", 1});
    QCOMPARE(qb.build(), "UPDATE books SET title = ? WHERE id = ?");
    QCOMPARE(qb.getValues(), QVariantList({"New Title", 1}));
  }

  void deleteFrom() {
    SqlQueryBuilder qb;
    qb.deleteFrom("books").where("id = ?").values({5});
    QCOMPARE(qb.build(), "DELETE FROM books WHERE id = ?");
    QCOMPARE(qb.getValues(), QVariantList{5});
  }

  void leftJoin() {
    SqlQueryBuilder qb;
    qb.select({"b.title", "a.name"})
        .from("books", "b")
        .leftJoin("authors", "a")
        .on("b.author_id = a.id");
    QCOMPARE(qb.build(),
             "SELECT b.title, a.name FROM books b LEFT JOIN authors a "
             "ON b.author_id = a.id");
  }

  void valuesOverwrite() {
    SqlQueryBuilder qb;
    qb.select().from("books").values({1, 2});
    QCOMPARE(qb.getValues(), QVariantList({1, 2}));
    qb.values({3});
    QCOMPARE(qb.getValues(), QVariantList{3});
  }
};

QTEST_GUILESS_MAIN(tst_SqlQueryBuilder)
#include "tst_SqlQueryBuilder.moc"
