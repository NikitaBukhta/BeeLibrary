#include "AppInitializer.hpp"
#include "AppEnvironment.hpp"

#include <QLoggingCategory>
#include <QQmlContext>

Q_LOGGING_CATEGORY(lcInit, "bl.core.init")

namespace bl::core {

AppInitializer::AppInitializer(QQmlApplicationEngine &engine)
    : _engine{engine} {}

void AppInitializer::initialize() {
  qCInfo(lcInit) << "Initializing application...";

  initDatabase();
  initModels();
  registerQmlTypes();
  exposeToQml();

  qCInfo(lcInit) << "Application initialized";
}

void AppInitializer::initDatabase() {
  _db = std::make_shared<DatabaseManager>(AppEnvironment::databasePath());
  _db->open();
  _db->runScript(":/db/init.sql");
  _db->runScript(":/db/test_data.sql");

  _bookTable = std::make_shared<services::BookTable>(_db);

  qCInfo(lcInit) << "Database layer ready";
}

void AppInitializer::initModels() {
  _bookListModel = std::make_unique<models::BookListModel>(_bookTable);

  _bookProxyModel = std::make_unique<models::BookProxyModel>();
  _bookProxyModel->setSourceModel(_bookListModel.get());

  _contextModel = std::make_unique<models::ContextModel>();

  qCInfo(lcInit) << "Models ready";
}

void AppInitializer::registerQmlTypes() {
  qmlRegisterUncreatableType<models::ContextModel>(
      "Library", 1, 0, "ContextModel",
      "ContextModel is not creatable — enums only");
}

void AppInitializer::exposeToQml() {
  auto *ctx = _engine.rootContext();
  ctx->setContextProperty("bookListModel", _bookListModel.get());
  ctx->setContextProperty("bookProxyModel", _bookProxyModel.get());
  ctx->setContextProperty("contextModel", _contextModel.get());

  qCInfo(lcInit) << "QML context properties registered";
}

} // namespace bl::core
