#include "osgearthrenderer2.h"
#include <osgEarth/EarthManipulator>
#include <osgEarth/LogarithmicDepthBuffer>
#include <osgEarth/TMS>
#include <osgViewer/ViewerEventHandlers>
#include <QOpenGLFramebufferObject>

#define IMAGERY_URL      "http://readymap.org/readymap/tiles/1.0.0/22/"
#define ELEVATION_URL    "http://readymap.org/readymap/tiles/1.0.0/116/"
#define BUILDINGS_URL    "/home/canhdx/OSG_Test/data/boston_buildings_utm19.shp"
#define RESOURCE_LIB_URL "/home/canhdx/OSG_Test/data/catalog.xml"
#define STREETS_URL      "/home/canhdx/OSG_Test/data/boston-scl-utm19n-meters.shp"
#define PARKS_URL        "/home/canhdx/OSG_Test/data/boston-parks.shp"
#define TREE_MODEL_URL   "/home/canhdx/OSG_Test/data/tree.osg"

OsgEarthRenderer2::OsgEarthRenderer2() {
    osgEarth::initialize();

    m_viewer = new osgViewer::Viewer;
    m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_viewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);

    initOsgEarthScene();
}

OsgEarthRenderer2::~OsgEarthRenderer2() {}

void OsgEarthRenderer2::initOsgEarthScene() {
    m_map = new osgEarth::Map;
    addImagery();
    addElevation();

    m_mapNode = new osgEarth::MapNode(m_map.get());
    osg::Group* root = new osg::Group;
    root->addChild(m_mapNode.get());

    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (manip) {
        manip->setViewpoint(osgEarth::Viewpoint("Home", -71.0763, 42.34425, 0, 24.261, -21.6, 3450.0), 5.0);
    }

    m_viewer->setSceneData(root);
    osgEarth::Util::LogarithmicDepthBuffer buf;
    buf.install(m_viewer->getCamera());
}

void OsgEarthRenderer2::addImagery() {
    auto* layer = new osgEarth::TMSImageLayer;
    layer->setURL(IMAGERY_URL);
    m_map->addLayer(layer);
}

void OsgEarthRenderer2::addElevation() {
    auto* layer = new osgEarth::TMSElevationLayer;
    layer->setURL(ELEVATION_URL);
    m_map->addLayer(layer);
}

void OsgEarthRenderer2::addBuildings() {

}

void OsgEarthRenderer2::addStreets() {

}

void OsgEarthRenderer2::addParks() {

}

QOpenGLFramebufferObject *OsgEarthRenderer2::createFramebufferObject(const QSize &size) {
    if (size.width() <= 0 || size.height() <= 0) {
        qWarning() << "Invalid FBO size:" << size;
        return nullptr;
    }
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    return new QOpenGLFramebufferObject(size, format);
}

void OsgEarthRenderer2::render() {
    m_viewer->frame();
    update();
}

void OsgEarthRenderer2::synchronize(QQuickFramebufferObject *item) {
    Q_UNUSED(item)
}
