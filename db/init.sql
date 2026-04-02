PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS books (
  id     INTEGER PRIMARY KEY AUTOINCREMENT,
  title  TEXT    NOT NULL,
  author TEXT    NOT NULL,
  year   INTEGER DEFAULT NULL,
  isbn   TEXT    DEFAULT NULL
);

CREATE INDEX IF NOT EXISTS ix_books_title  ON books(title);
CREATE INDEX IF NOT EXISTS ix_books_author ON books(author);
CREATE UNIQUE INDEX IF NOT EXISTS ux_books_isbn ON books(isbn);
