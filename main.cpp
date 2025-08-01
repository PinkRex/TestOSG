#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <SimpleOsgEarthItem.h>
#include <osgEarth/Registry>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    osgEarth::initialize();
    qmlRegisterType<SimpleOsgEarthItem>("OsgEarth", 1, 0, "SimpleOsgEarthItem");

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("/home/canhdx/OSG_Test/Main.qml"));
    return app.exec();
}
