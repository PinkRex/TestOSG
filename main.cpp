#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <osgEarth/Registry>
#include <QDebug>

#include <osgearthitem2.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    QGuiApplication app(argc, argv);
    osgEarth::initialize();
    qmlRegisterType<OsgEarthItem2>("OsgEarth2", 1, 0, "OsgEarthItem2");

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("/home/canhdx/OSG_Test/Main.qml"));
    return app.exec();
}
