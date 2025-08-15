#include "2D_Handler/OsgEarthRenderer_2D.h"
#include "3D_Handler/OsgEarthRenderer_3D.h"

#include <osgEarth/Map>
#include <osgEarth/ImageLayer>
#include <osgEarth/TMS>
#include <osgEarth/XYZ>
#include <osgEarth/Profile>
#include <osgGA/TrackballManipulator>

#include <osgEarth/EarthManipulator>
#include <osgEarth/LogarithmicDepthBuffer>
#include <osgEarth/FeatureModelLayer>
#include <osgEarth/OGRFeatureSource>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/TerrainEngineNode>

#include <QOpenGLFramebufferObject>
#include <QTimer>
#include <QDebug>

// #define IMAGERY_URL      "http://[abc].tile.openstreetmap.org/{z}/{x}/{y}.png"
#define IMAGERY_URL      "http://192.168.44.218:8881/google_satellite_no_hybrid/{z}/{x}/{y}.jpeg"
// #define IMAGERY_URL      "http://192.168.44.11:8080/tile/{z}/{x}/{y}.png"

OsgEarthRenderer_2D::OsgEarthRenderer_2D() {
    osgEarth::initialize();
    m_viewer = new osgViewer::Viewer;
    m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_viewer->getEventHandlers().clear();
    m_viewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 0;
    traits->width = 1024;
    traits->height = 768;
    traits->windowDecoration = false;
    traits->doubleBuffer = false;
    traits->sharedContext = 0;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> gc = new osgViewer::GraphicsWindowEmbedded(traits.get());
    if (!gc.valid()) {
        qWarning() << "Failed to create embedded GraphicsWindow!";
        return;
    }

    m_viewer->getCamera()->setGraphicsContext(gc.get());
    m_viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    m_viewer->getCamera()->setDrawBuffer(GL_BACK);
    m_viewer->getCamera()->setReadBuffer(GL_BACK);

    initOsgEarthScene();
}

OsgEarthRenderer_2D::~OsgEarthRenderer_2D() {}

void OsgEarthRenderer_2D::initOsgEarthScene() {
    m_map = new osgEarth::Map;
    m_map->setProfile(osgEarth::Profile::create(osgEarth::Profile::SPHERICAL_MERCATOR));
    addImagery();

    m_mapNode = new osgEarth::MapNode(m_map.get());
    osg::Group* root = new osg::Group;
    root->addChild(m_mapNode.get());

    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (manip) {
        auto* settings = manip->getSettings();
        settings->setMouseSensitivity(0.0025);
        settings->setThrowingEnabled(false);
        settings->setZoomToMouse(true);

        manip->applySettings(settings);
    }

    m_viewer->setSceneData(root);
    osgEarth::Util::LogarithmicDepthBuffer buf;
    buf.install(m_viewer->getCamera());
}

void OsgEarthRenderer_2D::addImagery() {
    auto* layer = new osgEarth::XYZImageLayer;
    layer->setURL(IMAGERY_URL);
    m_map->addLayer(layer);
}

QOpenGLFramebufferObject *OsgEarthRenderer_2D::createFramebufferObject(const QSize &size) {
    if (size.width() <= 0 || size.height() <= 0) {
        qWarning() << "Invalid FBO size:" << size;
        return nullptr;
    }
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    format.setTextureTarget(GL_TEXTURE_2D);
    return new QOpenGLFramebufferObject(size, format);
}

void OsgEarthRenderer_2D::render() {
    m_viewer->frame();
    update();
}

void OsgEarthRenderer_2D::synchronize(QQuickFramebufferObject *item) {
    QSize size = item->size().toSize();
    if (size.isEmpty()) {
        qWarning() << "Empty size in synchronize";
        return;
    }
    double w = size.width();
    double h = size.height();
    osg::Camera* camera = m_viewer->getCamera();
    if (camera && camera->getViewport()) {
        camera->getViewport()->setViewport(0, 0, size.width(), size.height());
        camera->setProjectionMatrixAsOrtho2D(0, w, 0, h);
        // camera->setViewMatrix(osg::Matrix::identity()); // nhìn từ trên xuống
        camera->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OFF);
        camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
        camera->setRenderOrder(osg::Camera::POST_RENDER);

        camera->setViewMatrix(camera->getViewMatrix());
    }
}

void OsgEarthRenderer_2D::handleMouseEvent(QMouseEvent* event) {
    if (!m_viewer) return;
    osgGA::GUIEventAdapter::EventType eaType;

    switch (event->type()) {
    case QEvent::MouseButtonPress: eaType = osgGA::GUIEventAdapter::PUSH; break;
    case QEvent::MouseMove:        eaType = osgGA::GUIEventAdapter::DRAG; break;
    case QEvent::MouseButtonRelease:eaType = osgGA::GUIEventAdapter::RELEASE; break;
    default: return;
    }

    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MiddleButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton:
    case Qt::BackButton:
    case Qt::ForwardButton:
    case Qt::TaskButton:
    case Qt::ExtraButton4:
    case Qt::ExtraButton5:
    case Qt::ExtraButton6:
    case Qt::ExtraButton7:
    case Qt::ExtraButton8:
    case Qt::ExtraButton9:
    case Qt::ExtraButton10:
    case Qt::ExtraButton11:
    case Qt::ExtraButton12:
    case Qt::ExtraButton13:
    case Qt::ExtraButton14:
    case Qt::ExtraButton15:
    case Qt::ExtraButton16:
    case Qt::ExtraButton17:
    case Qt::ExtraButton18:
    case Qt::ExtraButton19:
    case Qt::ExtraButton20:
    case Qt::ExtraButton21:
    case Qt::ExtraButton22:
    case Qt::ExtraButton23:
    case Qt::ExtraButton24:
    case Qt::AllButtons:
    case Qt::MouseButtonMask:
        break;
    }

    auto queue = m_viewer->getEventQueue();
    if (queue) {
        queue->mouseMotion(event->position().x(), event->position().y());
        if (eaType == osgGA::GUIEventAdapter::PUSH) {
            queue->mouseButtonPress(event->position().x(), event->position().y(), button);
        } else if (eaType == osgGA::GUIEventAdapter::RELEASE) {
            queue->mouseButtonRelease(event->position().x(), event->position().y(), button);
        }
    }
}

void OsgEarthRenderer_2D::handleMousePressEvent(QMouseEvent *event) {
    if (!m_mapNode) return;

    osgViewer::View* view = m_viewer.get();
    if (!view) return;

    osg::Viewport* vp = view->getCamera()->getViewport();
    if (!vp) return;

    int fboW = this->framebufferObject()->width();
    int fboH = this->framebufferObject()->height();

    // double osgX = event->pos().x() * (vp->width() / (double)fboW);
    // double osgY = (fboH - event->pos().y()) * (vp->height() / (double)fboH);

    double osgX = vp->x() + (event->pos().x() * (vp->width()  / static_cast<double>(fboW)));
    double osgY = vp->y() + ((fboH - event->pos().y()) * (vp->height() / static_cast<double>(fboH)));

    osgEarth::GeoPoint geo;
    auto wgs84 = osgEarth::SpatialReference::get("wgs84");
    double lon, lat;
    auto hit = m_mapNode->getGeoPointUnderMouse(view, osgX, osgY);

    if (hit.isValid())
    {
        hit.transform(wgs84, geo);
        geo.makeGeographic();
        lon = geo.x();
        lat = geo.y();

        qDebug() << "Lat:" << lat << "Lon:" << lon;
        renderer_3D->setCoordinate(lat, lon);
    }
    else
    {
        qDebug() << "No hit";
    }
}

void OsgEarthRenderer_2D::focusHanoi() {
    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (manip) {
        // Boston: Lat: -71.0763 Lon: 42.34425
        manip->setViewpoint(osgEarth::Viewpoint(
                                "Ha Noi",   // label
                                105.84,   // longitude
                                21.0078,   // latitude
                                0,          // altitude (m)
                                0,          // heading (deg)
                                -90.0,      // pitch (deg)
                                5000.0      // range (m)
                                ), 3.0);    // 3s animation
    }
}

void OsgEarthRenderer_2D::handleWheelEvent(QWheelEvent* event) {
    auto queue = m_viewer->getEventQueue();
    if (!queue) return;

    queue->mouseMotion(event->position().x(), event->position().y());
    float delta = event->angleDelta().y() / 120.0f;
    if (delta > 0) {
        queue->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
    } else {
        queue->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
}
