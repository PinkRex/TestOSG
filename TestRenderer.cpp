// #include "3D_Handler/OsgEarthRenderer_3D.h"
// #include <osgEarth/EarthManipulator>
// #include <osgEarth/LogarithmicDepthBuffer>
// #include <osgEarth/ImageLayer>
// #include <osgEarth/TMS>
// #include <osgEarth/FeatureModelLayer>
// #include <osgEarth/OGRFeatureSource>
// #include <osgViewer/ViewerEventHandlers>
// #include <osgEarth/TerrainEngineNode>

// #include <QOpenGLFramebufferObject>
// #include <QTimer>

// #define IMAGERY_URL      "http://readymap.org/readymap/tiles/1.0.0/22/"
// #define ELEVATION_URL    "http://readymap.org/readymap/tiles/1.0.0/116/"
// #define BUILDINGS_URL    "/home/canhdx/TestOSG/data/boston_buildings_utm19.shp"
// #define RESOURCE_LIB_URL "/home/canhdx/TestOSG/data/catalog.xml"
// #define STREETS_URL      "/home/canhdx/TestOSG/data/boston-scl-utm19n-meters.shp"
// #define PARKS_URL        "/home/canhdx/TestOSG/data/boston-parks.shp"
// #define TREE_MODEL_URL   "/home/canhdx/TestOSG/data/tree.osg"

// OsgEarthRenderer_3D::OsgEarthRenderer_3D() {
//     osgEarth::initialize();
//     m_viewer = new osgViewer::Viewer;
//     m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
//     m_viewer->getEventHandlers().clear();
//     m_viewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);
//     m_viewer->addEventHandler(new osgViewer::StatsHandler);

//     osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
//     traits->windowDecoration = false;
//     traits->doubleBuffer = true;
//     traits->sharedContext = 0;

//     osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> gc = new osgViewer::GraphicsWindowEmbedded(traits.get());
//     if (!gc.valid()) {
//         qWarning() << "Failed to create embedded GraphicsWindow!";
//         return;
//     }

//     m_viewer->getCamera()->setGraphicsContext(gc.get());
//     m_viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
//     m_viewer->getCamera()->setDrawBuffer(GL_BACK);
//     m_viewer->getCamera()->setReadBuffer(GL_BACK);

//     initOsgEarthScene();
// }

// OsgEarthRenderer_3D::~OsgEarthRenderer_3D() {}

// void OsgEarthRenderer_3D::initOsgEarthScene() {
//     m_map = new osgEarth::Map;
//     addImagery();
//     addElevation();
//     addBuildings();
//     addStreets();
//     addParks();

//     _isBusy = false;
//     m_mapNode = new osgEarth::MapNode(m_map.get());
//     osg::Group* root = new osg::Group;
//     root->addChild(m_mapNode.get());

//     auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
//     if (manip) {
//         manip->setViewpoint(osgEarth::Viewpoint("Home", -71.0763, 42.34425, 0, 24.261, -21.6, 3450.0), 5.0);
//         auto* settings = manip->getSettings();
//         settings->setMouseSensitivity(0.001);
//         settings->setThrowingEnabled(false);
//         settings->setZoomToMouse(true);

//         manip->applySettings(settings);
//     }

//     m_viewer->setSceneData(root);
//     osgEarth::Util::LogarithmicDepthBuffer buf;
//     buf.install(m_viewer->getCamera());
// }

// void OsgEarthRenderer_3D::addImagery() {
//     auto* layer = new osgEarth::TMSImageLayer;
//     layer->setURL(IMAGERY_URL);
//     m_map->addLayer(layer);
// }

// void OsgEarthRenderer_3D::addElevation() {
//     auto* layer = new osgEarth::TMSElevationLayer;
//     layer->setURL(ELEVATION_URL);
//     m_map->addLayer(layer);
// }

// QOpenGLFramebufferObject *OsgEarthRenderer_3D::createFramebufferObject(const QSize &size) {
//     if (size.width() <= 0 || size.height() <= 0) {
//         qWarning() << "Invalid FBO size:" << size;
//         return nullptr;
//     }
//     QOpenGLFramebufferObjectFormat format;
//     format.setAttachment(QOpenGLFramebufferObject::Depth);
//     return new QOpenGLFramebufferObject(size, format);
// }

// void OsgEarthRenderer_3D::render() {
//     m_viewer->frame();
//     update();
// }

// void OsgEarthRenderer_3D::synchronize(QQuickFramebufferObject *item) {
//     QSize size = item->size().toSize();
//     if (size.isEmpty()) {
//         qWarning() << "Empty size in synchronize";
//         return;
//     }
//     osg::Camera* camera = m_viewer->getCamera();
//     if (camera && camera->getViewport()) {
//         camera->getViewport()->setViewport(0, 0, size.width(), size.height());
//         camera->setProjectionMatrixAsPerspective(30.0, size.width() / (double)size.height(), 1.0, 10000.0);

//         camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
//         camera->setRenderOrder(osg::Camera::POST_RENDER);

//         camera->setViewMatrix(camera->getViewMatrix());
//     }
// }
