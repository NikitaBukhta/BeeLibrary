#ifndef BEELIBRARY_CORE_APPINITIALIZER_HPP
#define BEELIBRARY_CORE_APPINITIALIZER_HPP

#include "DatabaseManager.hpp"
#include "models/BookListModel.hpp"
#include "models/BookProxyModel.hpp"
#include "models/ContextModel.hpp"
#include "services/BookTable.hpp"

#include <QQmlApplicationEngine>
#include <memory>

namespace bl::core {

class AppInitializer {
public:
  explicit AppInitializer(QQmlApplicationEngine &engine);

  void initialize();

private:
  void initDatabase();
  void initModels();
  void registerQmlTypes();
  void exposeToQml();

  QQmlApplicationEngine &_engine;

  std::shared_ptr<DatabaseManager> _db;
  std::shared_ptr<services::BookTable> _bookTable;
  std::unique_ptr<models::BookListModel> _bookListModel;
  std::unique_ptr<models::BookProxyModel> _bookProxyModel;
  std::unique_ptr<models::ContextModel> _contextModel;
};

} // namespace bl::core

#endif // BEELIBRARY_CORE_APPINITIALIZER_HPP
