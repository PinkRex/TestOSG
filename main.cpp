#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <osgEarth/Registry>
#include <QDebug>
#include <QSurfaceFormat>

#include <osgearthitem2.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    qputenv("QSG_RENDER_LOOP", "basic");

    // osg::setNotifyLevel(osg::DEBUG_FP);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 1);
    QSurfaceFormat::setDefaultFormat(format);


    QGuiApplication app(argc, argv);
    osgEarth::initialize();
    qmlRegisterType<OsgEarthItem2>("OsgEarth2", 1, 0, "OsgEarthItem2");

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("/home/canhdx/TestOSG/Main.qml"));
    return app.exec();
}
