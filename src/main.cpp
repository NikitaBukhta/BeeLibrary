#include "core/AppEnvironment.hpp"
#include "core/AppInitializer.hpp"

#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  bl::core::AppEnvironment::installFileLogger();

#ifdef QT_NO_DEBUG
  QLoggingCategory::setFilterRules("bl.*=false");
#endif

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  bl::core::AppInitializer initializer(engine);
  initializer.initialize();

  engine.loadFromModule("Library", "Main");

  return app.exec();
}
