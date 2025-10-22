#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>
#include <QtQml>

#include "SceneManager.h"
#include "Primitive.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<SceneManager>("SceneManager", 1, 0, "SceneManager");
    SceneManager sceneManager;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("SceneManager", &sceneManager);

    engine.loadFromModule("basics", "Main");

    qDebug() << "Application started successfully";

    return app.exec();
}
