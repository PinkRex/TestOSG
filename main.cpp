#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <SimpleOsgEarthItemRenderer.h>
#include <osgEarth/Registry>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    osgEarth::initialize();
    qmlRegisterType<SimpleOsgEarthItemRenderer>("OsgEarth", 1, 0, "SimpleOsgEarthItemRenderer");

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("/home/canhdx/OSG_Test/Main.qml"));
    return app.exec();
}
