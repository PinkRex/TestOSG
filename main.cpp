#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <osgEarth/Registry>
#include <QDebug>
#include <QSurfaceFormat>

#include <OsgEarthItem_3D.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    qputenv("QSG_RENDER_LOOP", "basic");

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 1);
    QSurfaceFormat::setDefaultFormat(format);


    QGuiApplication app(argc, argv);
    osgEarth::initialize();
    qmlRegisterType<OsgEarthItem_3D>("OsgEarth_3D", 1, 0, "OsgEarthItem_3D");

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("/home/canhdx/TestOSG/Main.qml"));
    return app.exec();
}
