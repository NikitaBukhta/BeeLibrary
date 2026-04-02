# BeeLibrary — Architecture

## Overview

BeeLibrary is a Qt 6.8 QML desktop application built with C++17. The project follows a layered architecture separating business logic (C++) from presentation (QML).

## Tech Stack

| Layer          | Technology                |
|----------------|---------------------------|
| UI             | QML / Qt Quick            |
| Application    | C++17, Qt 6.8             |
| Database       | SQLite (Qt SQL)           |
| Build          | CMake 3.16+, vcpkg        |
| Packaging      | Inno Setup (Windows)      |
| Build scripts  | Python (bootstrap.py)     |

## Project Structure

```
BeeLibrary/
├── src/                    C++ source code
│   ├── main.cpp               Application entry point
│   ├── core/                  Infrastructure: DB, SQL builder, app init
│   │   ├── AppInitializer        Startup orchestration
│   │   ├── AppEnvironment        App paths and file logger
│   │   ├── DatabaseManager       SQLite connection and queries
│   │   └── SqlQueryBuilder       Fluent parameterized SQL builder
│   ├── models/                QAbstractListModel subclasses exposed to QML
│   │   ├── BookListModel         Book CRUD model (Q_INVOKABLE)
│   │   ├── BookProxyModel        Search/filter proxy (QSortFilterProxyModel)
│   │   └── ContextModel          Page navigation with stack
│   └── services/              Data access and persistence
│       ├── BookTable             DAO: getAllBooks, addBook, updateBook, deleteBook, isbnExists
│       └── BookDTO               Data struct + mapper
├── qml/                    QML user interface
│   ├── Main.qml               Root ApplicationWindow + Loader
│   ├── views/                 Full-screen pages
│   │   ├── BookListPage          Book list + search + delete dialog
│   │   ├── BookEditPage          Add/edit form with validation
│   │   └── BookListView          ListView with BookCard delegates
│   ├── components/            Reusable UI components
│   │   ├── BookCard              Book display card with edit/delete
│   │   └── SearchBar             Debounced search input
│   ├── library/               Generic UI primitives
│   │   ├── PageTemplate          Scrollable page container
│   │   ├── Panel                 Titled card container
│   │   ├── Icon                  Icon/badge element
│   │   ├── ImageTextButton       Vertical icon + text button
│   │   └── TextIconRow           Horizontal text + icon row
│   └── utils/                 QML singletons
│       ├── Geometry              Layout metrics and sizing
│       └── Styles                Color palette
├── db/                     SQL scripts
│   ├── init.sql               Schema: books table + indexes
│   ├── test_data.sql          20 sample books
│   └── db_scripts.qrc        Qt resource manifest
├── tests/                  Unit and integration tests (Qt Test)
├── docs/                   Documentation
├── ai_context/             AI assistance context files
├── buildtools/             Python build system (CLI, providers, commands)
├── installer/              Inno Setup script and qt.conf
├── CMakeLists.txt          CMake project configuration
├── CMakePresets.json        Debug/Release build presets
├── vcpkg.json              vcpkg dependency manifest
└── bootstrap.py            Build entry point
```

## Architectural Layers

```
┌─────────────────────────────────────────┐
│               QML UI Layer              │
│  views/  components/  library/  utils/  │
└──────────────────┬──────────────────────┘
                   │  property bindings, Q_INVOKABLE calls
┌──────────────────┴──────────────────────┐
│             C++ Models Layer            │
│            src/models/                  │
│   BookListModel — CRUD + validation     │
│   BookProxyModel — search/filter        │
│   ContextModel — page navigation        │
└──────────────────┬──────────────────────┘
                   │  method calls
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
│   AppInitializer — startup chain        │
│   AppEnvironment — paths, logging       │
│   DatabaseManager — SQLite operations   │
│   SqlQueryBuilder — fluent SQL builder  │
└─────────────────────────────────────────┘
```

## Data Flow

1. **QML** binds to properties and roles exposed by **Models**.
2. **Models** call **Services** to load, save, or transform data.
3. **Services** handle persistence (SQLite) via **Core** infrastructure.
4. **Core** provides database access, SQL building, and app configuration.

Changes propagate upward through Qt's signal/slot mechanism: a Service modifies data, the Model calls `beginResetModel()`/`endResetModel()`, and QML reacts automatically via property bindings.

## Namespaces

| Directory | Namespace | Purpose |
|-----------|-----------|---------|
| src/core/ | `bl::core` | Infrastructure, shared types |
| src/services/ | `bl::services` | Data access, DTOs |
| src/models/ | `bl::models` | QML-exposed models |

## Page Navigation

ContextModel manages a page stack with hierarchy levels:
- Level 1: BookListPage (home)
- Level 2: BookEditPage (child of list)

Navigation: `contextModel.currentPage = ContextModel.BOOK_EDIT_PAGE`
Back: `contextModel.openPreviousPage()` (pops stack)
Page loading: `Main.qml` has a Loader bound to `contextModel.currentPagePath` (QUrl).

Inter-page data passes through `ApplicationWindow.window.editBookId`.

## QML Module

All QML files belong to a single module with URI `Library`. The module is registered in CMake via `qt_add_qml_module`. Singletons (`Geometry`, `Styles`) are marked with `QT_QML_SINGLETON_TYPE TRUE`.

New QML files placed in `qml/` are auto-discovered by CMake's `GLOB_RECURSE`.

## C++ / QML Integration

C++ types are exposed to QML via context properties set in `AppInitializer::exposeToQml()`:
- `bookListModel` — BookListModel (CRUD operations)
- `bookProxyModel` — BookProxyModel (search/filter for ListView)
- `contextModel` — ContextModel (page navigation)

ContextModel's `PageEnum` is available in QML via `qmlRegisterUncreatableType`.

## Validation Strategy

- **QML side**: Input correctness — disable Save when required fields empty, IntValidator for year, RegularExpressionValidator for ISBN digits
- **C++ side**: Business rules — title/author required, year range, ISBN uniqueness via DB query
- **SQL side**: Safety net — NOT NULL constraints, UNIQUE index on isbn

## Logging

Qt logging categories by layer:
- `bl.core.db` — database operations
- `bl.core.init` — initialization steps
- `bl.core.env` — environment/paths
- `bl.services.books` — BookTable CRUD
- `bl.models.books` — BookListModel operations
- `bl.models.context` — page navigation

Debug: all logs enabled (console + file). Release: `bl.*` disabled.

## Build System

The Python-based `bootstrap.py` wraps CMake and vcpkg:

| Command                            | Description                  |
|------------------------------------|------------------------------|
| `python bootstrap.py bootstrap`    | Install deps, configure      |
| `python bootstrap.py compile`      | Build (Debug by default)     |
| `python bootstrap.py run`          | Run the built executable     |
| `python bootstrap.py package`      | Create Windows installer     |
| `python bootstrap.py clean`        | Remove all build artifacts   |

Add `--release` to `bootstrap`, `compile`, or `run` for a Release build.

## Dependencies

Managed via vcpkg (see `vcpkg.json`):

- **qtbase 6.8.0** — Core, GUI, Network, SQL, Widgets
- **qtdeclarative 6.8.0** — QML, Qt Quick

Dependencies are installed to `~/BeeLibrary-dependencies/` to avoid re-downloading across clean builds.
