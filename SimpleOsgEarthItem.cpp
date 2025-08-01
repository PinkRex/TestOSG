#include "SimpleOsgEarthItem.h"

#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QTimer>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/Vec3d>
#include <osgDB/ReadFile>

#include <osgEarth/Registry>
#include <osgEarth/MapNode>
#include <osgEarth/EarthManipulator>

class SimpleOsgEarthRenderer : public QQuickFramebufferObject::Renderer {
public:
    explicit SimpleOsgEarthRenderer(SimpleOsgEarthItem* item)
        : m_item(item), m_lastSize(0, 0)
    {
        m_viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
        // m_viewer.setUpViewerAsEmbeddedInWindow(0, 0, 640, 480);
        m_viewer.setCameraManipulator(new osgGA::TrackballManipulator());
        m_viewer.setUpViewerAsEmbeddedInWindow(0, 0, 1, 1);
        m_viewer.realize();
        // m_manip = new osgEarth::Util::EarthManipulator();
        // m_viewer.setCameraManipulator(m_manip.get());

        // m_viewer.setLightingMode(osg::View::NO_LIGHT);
        // m_viewer.getCamera()->setClearColor(osg::Vec4(0.1f, 0.1f, 0.2f, 1.0f));
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void render() override {
        if (!m_initialized && !m_item->earthFile().isEmpty()) {
            loadEarthFile(m_item->earthFile());
            m_initialized = true;
            m_firstTime = true;
        }

        QSizeF newSize = m_item->size();
        if (newSize != m_lastSize) {
            m_viewer.getCamera()->setViewport(0, 0, newSize.width(), newSize.height());
            m_lastSize = newSize;
        }

        qDebug() << "Frame:" << frameCount++
                 << "sceneData:" << m_viewer.getSceneData()
                 << "manip:" << m_viewer.getCameraManipulator();

        m_viewer.frame();

        if (m_firstTime) {
            setupCamera();
            m_firstTime = false;
        }

        update();
    }

    void setupCamera() {
        auto manip = m_viewer.getCameraManipulator();
        if (manip) {
            manip->home(0.0);
        }
    }

private:
    void loadEarthFile(const QString& path) {
        m_root = nullptr;
        m_root = dynamic_cast<osg::Group*>(osgDB::readNodeFile(path.toStdString()));
        if (!m_root) {
            qWarning("Failed to load .earth file: %s", qUtf8Printable(path));
            return;
        }
        m_viewer.setSceneData(m_root.get());
        m_viewer.addEventHandler(new osgViewer::StatsHandler);

        // setupCamera();
    }

    SimpleOsgEarthItem* m_item;
    osgViewer::Viewer m_viewer;
    osg::ref_ptr<osg::Node> m_root;
    osg::ref_ptr<osgGA::CameraManipulator> m_manip;
    bool m_initialized = false;
    bool m_firstTime = true;
    int frameCount = 0;
    QSizeF m_lastSize;
};

SimpleOsgEarthItem::SimpleOsgEarthItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
{
    setMirrorVertically(true);
}

QQuickFramebufferObject::Renderer* SimpleOsgEarthItem::createRenderer() const {
    return new SimpleOsgEarthRenderer(const_cast<SimpleOsgEarthItem*>(this));
}

void SimpleOsgEarthItem::setEarthFile(const QString& file) {
    if (m_earthFile != file) {
        m_earthFile = file;
        emit earthFileChanged();
    }
}
