# BeeLibrary — Architecture

## Overview

BeeLibrary is a Qt 6.8 QML desktop application built with C++17. The project follows an MVC architecture separating business logic (C++) from presentation (QML).

## Tech Stack

| Layer          | Technology                |
|----------------|---------------------------|
| UI             | QML / Qt Quick            |
| Application    | C++17, Qt 6.8             |
| Database       | SQLite (Qt SQL)           |
| Build          | CMake 3.16+, vcpkg        |
| Tests          | Qt Test                   |
| Packaging      | Inno Setup (Windows)      |
| Build scripts  | Python (bootstrap.py)     |

## Project Structure

```
BeeLibrary/
├── src/                    C++ source code
│   ├── main.cpp               Application entry point
│   ├── core/                  Infrastructure
│   │   ├── AppInitializer        Startup, DI, QML registration
│   │   ├── AppEnvironment        Paths, file logger, log rotation
│   │   ├── DatabaseManager       SQLite: select, execute, insert
│   │   └── SqlQueryBuilder       Fluent parameterized SQL builder
│   ├── controllers/           Application logic
│   │   ├── BookFormController    Form submit, validation, ISBN, yearMin/Max
│   │   └── NavigationController Stack-based page navigation
│   ├── models/                Data models for QML
│   │   ├── BookListModel         Book list (QAbstractListModel)
│   │   └── BookProxyModel        Search, filter, sort (QSortFilterProxyModel)
│   └── services/              Data access
│       ├── BookTable             DAO: CRUD + isbnExists
│       └── BookDTO               Data struct + toMap/fromMap
├── qml/                    QML user interface
│   ├── Main.qml               Root ApplicationWindow + Loader
│   ├── pages/                 Page-scoped modules
│   │   ├── booklist/             Book list page
│   │   │   ├── BookListPage        Main page layout
│   │   │   ├── BookListView        ListView + delegate + empty state
│   │   │   ├── BookCard            Book item delegate
│   │   │   ├── SearchBar           Debounced search input
│   │   │   ├── SortBar             Sort field + asc/desc toggle
│   │   │   ├── ErrorBanner         Error message banner
│   │   │   └── DeleteBookDialog    Delete confirmation dialog
│   │   └── bookedit/             Book edit/add page
│   │       ├── BookEditPage        Form page layout
│   │       ├── BookForm            Form fields container
│   │       └── FormField           Reusable label + input component
│   └── utils/                 QML singletons
│       ├── Geometry              Spacing, radius, font scale tokens
│       └── Styles                Color palette, opacity tokens
├── tests/                  Unit and integration tests (Qt Test)
│   ├── tst_SqlQueryBuilder      Query builder tests
│   ├── tst_BookDTO              DTO mapping tests
│   ├── tst_NavigationController Navigation stack tests
│   ├── tst_BookTable            DB integration tests (in-memory SQLite)
│   ├── tst_BookFormController   Form validation/submit tests
│   └── tst_BookProxyModel       Filter + sort tests
├── db/                     SQL scripts
│   ├── init.sql               Schema: books table + indexes
│   ├── test_data.sql          Sample books (debug builds only)
│   └── db_scripts.qrc        Qt resource manifest
├── assets/                 Application assets
│   ├── icon.ico               Application icon
│   └── app.rc                 Windows resource file
├── docs/                   Documentation
├── buildtools/             Python build system
├── installer/              Inno Setup script and qt.conf
├── CMakeLists.txt          CMake configuration
├── CMakePresets.json        Debug/Release presets
├── vcpkg.json              vcpkg dependencies
└── bootstrap.py            Build entry point
```

## Architectural Layers

```
┌─────────────────────────────────────────┐
│               QML UI Layer              │
│         pages/  utils/                  │
└──────────────────┬──────────────────────┘
                   │  context properties, Q_INVOKABLE calls
┌──────────────────┴──────────────────────┐
│          C++ Controllers Layer          │
│          src/controllers/               │
│   BookFormController — form logic       │
│   NavigationController — page navigation│
└──────────────────┬──────────────────────┘
                   │
┌──────────────────┴──────────────────────┐
│            C++ Models Layer             │
│            src/models/                  │
│   BookListModel — data for ListView     │
│   BookProxyModel — search/filter/sort   │
└──────────────────┬──────────────────────┘
                   │
┌──────────────────┴──────────────────────┐
│           C++ Services Layer            │
│           src/services/                 │
│   BookTable — DAO (database queries)    │
│   BookDTO — data transfer object        │
└──────────────────┬──────────────────────┘
                   │
┌──────────────────┴──────────────────────┐
│              C++ Core Layer             │
│              src/core/                  │
│   AppInitializer — startup, DI          │
│   AppEnvironment — paths, logging       │
│   DatabaseManager — SQLite operations   │
│   SqlQueryBuilder — fluent SQL builder  │
└─────────────────────────────────────────┘
```

## C++ / QML Integration

C++ types are exposed to QML via context properties set in `AppInitializer::registerQmlTypes()`:

| Context property      | C++ class            | Purpose                        |
|-----------------------|----------------------|--------------------------------|
| `bookListModel`       | BookListModel        | Book list data + delete        |
| `bookProxyModel`      | BookProxyModel       | Search, filter, sort           |
| `bookFormController`  | BookFormController   | Form submit, validation, state |
| `navigationController`| NavigationController | Page navigation                |

Enum access via `qmlRegisterUncreatableType`: `BookListModel.TitleRole`, `NavigationController.BOOK_EDIT_PAGE`.

Signal wiring in AppInitializer: `bookFormController::bookSaved → bookListModel::refresh`.

## Page Navigation

NavigationController manages a page stack with hierarchy levels:
- Level 1: BookListPage (home)
- Level 2: BookEditPage (child)

```qml
navigationController.currentPage = NavigationController.BOOK_EDIT_PAGE
navigationController.goBack()
```

Main.qml uses a Loader bound to `navigationController.currentPagePath`.

## DatabaseManager API

| Method                  | Returns          | Transaction | Use case          |
|-------------------------|------------------|-------------|-------------------|
| `select(builder)`       | `QList<QVariantMap>` | No      | SELECT queries    |
| `execute(builder)`      | `int` (affected rows) | Yes   | UPDATE, DELETE    |
| `insert(builder)`       | `qint64` (last ID)   | Yes   | INSERT            |
| `exec(sql)`             | `bool`           | Yes         | Raw SQL, scripts  |

## Validation Strategy

- **QML**: Input correctness — disable Save when required fields empty, IntValidator for year, RegularExpressionValidator for ISBN digits
- **C++ (BookFormController)**: Business rules — title/author required, year range (1..current year), ISBN uniqueness via DB query
- **SQL**: Safety net — NOT NULL constraints, UNIQUE index on isbn

## Logging

Qt logging categories by layer:

| Category                  | Source               |
|---------------------------|----------------------|
| `bl.core.db`              | DatabaseManager      |
| `bl.core.init`            | AppInitializer       |
| `bl.core.env`             | AppEnvironment       |
| `bl.services.books`       | BookTable            |
| `bl.models.books`         | BookListModel        |
| `bl.controllers.bookform` | BookFormController   |
| `bl.controllers.navigation` | NavigationController |

Debug: all logs to console + file. Release: only warnings and above to file.
Log files auto-rotate (files older than 7 days deleted on startup).

## Build System

| Command                                    | Description                    |
|--------------------------------------------|--------------------------------|
| `python bootstrap.py bootstrap`            | Install deps, configure        |
| `python bootstrap.py bootstrap -DVAR=VAL`  | Configure with CMake flags     |
| `python bootstrap.py compile`              | Build (Debug by default)       |
| `python bootstrap.py run`                  | Run with correct env           |
| `python bootstrap.py test`                 | Run unit tests                 |
| `python bootstrap.py package`              | Create Windows installer       |
| `python bootstrap.py format`               | Format C++ with clang-format   |
| `python bootstrap.py clean`                | Remove all build artifacts     |

Add `--release` to `bootstrap`, `compile`, `run`, or `test` for Release builds.

## CMake Targets

| Target           | Type            | Links to            |
|------------------|-----------------|----------------------|
| `BeeLibraryCore` | Static library  | Qt6::Core, Qt6::Sql  |
| `BeeLibrary`     | Executable      | BeeLibraryCore, Qt6::Quick |
| `tst_*`          | Test executables | BeeLibraryCore, Qt6::Test  |

Tests are controlled by `BUILD_TESTS` option (ON by default, `-DBUILD_TESTS=OFF` to disable).

## Namespaces

| Directory         | Namespace          | Purpose              |
|-------------------|--------------------|----------------------|
| src/core/         | `bl::core`         | Infrastructure       |
| src/services/     | `bl::services`     | Data access, DTOs    |
| src/models/       | `bl::models`       | QML data models      |
| src/controllers/  | `bl::controllers`  | Application logic    |

## Dependencies

Managed via vcpkg (see `vcpkg.json`):

- **qtbase 6.8.0** — Core, GUI, Network, SQL, Widgets
- **qtdeclarative 6.8.0** — QML, Qt Quick

Dependencies are installed to `~/BeeLibrary-dependencies/` to avoid re-downloading across clean builds.
