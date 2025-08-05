#include "osgearthrenderer2.h"
#include <osgEarth/EarthManipulator>
#include <osgEarth/LogarithmicDepthBuffer>
#include <osgEarth/ImageLayer>
#include <osgEarth/TMS>
#include <osgEarth/FeatureModelLayer>
#include <osgEarth/OGRFeatureSource>

#include <osgViewer/ViewerEventHandlers>
#include <QOpenGLFramebufferObject>

#define IMAGERY_URL      "http://readymap.org/readymap/tiles/1.0.0/22/"
#define ELEVATION_URL    "http://readymap.org/readymap/tiles/1.0.0/116/"
#define BUILDINGS_URL    "/home/canhdx/TestOSG/data/boston_buildings_utm19.shp"
#define RESOURCE_LIB_URL "/home/canhdx/TestOSG/data/catalog.xml"
#define STREETS_URL      "/home/canhdx/TestOSG/data/boston-scl-utm19n-meters.shp"
#define PARKS_URL        "/home/canhdx/TestOSG/data/boston-parks.shp"
#define TREE_MODEL_URL   "/home/canhdx/TestOSG/data/tree.osg"

OsgEarthRenderer2::OsgEarthRenderer2() {
    osgEarth::initialize();
    // osgEarth::setNotifyLevel(osg::DEBUG_INFO);

    m_viewer = new osgViewer::Viewer;
    m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_viewer->getEventHandlers().clear();
    m_viewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 0;
    traits->width = 1;
    traits->height = 1;
    traits->windowDecoration = false;
    traits->doubleBuffer = true;
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

OsgEarthRenderer2::~OsgEarthRenderer2() {}

void OsgEarthRenderer2::initOsgEarthScene() {
    m_map = new osgEarth::Map;
    addImagery();
    addElevation();
    addBuildings();
    addStreets();
    addParks();

    m_mapNode = new osgEarth::MapNode(m_map.get());
    osg::Group* root = new osg::Group;
    root->addChild(m_mapNode.get());

    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (manip) {
        // manip->setViewpoint(osgEarth::Viewpoint("Home", -71.0763, 42.34425, 0, 24.261, -21.6, 3450.0), 5.0);
        manip->setViewpoint(osgEarth::Viewpoint("Home", -71.0763, 42.34425, 0, 0, -45.0, 5000.0), 5.0);

        auto* settings = manip->getSettings();  // Clone để giữ config gốc

        settings->setMouseSensitivity(0.01);         // Giảm độ nhạy
        settings->setThrowingEnabled(false);        // Tắt "quán tính"

        manip->applySettings(settings);
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
    osgEarth::OGRFeatureSource* data = new osgEarth::OGRFeatureSource();
    data->setName("buildings-data");
    data->setURL(BUILDINGS_URL);

    qDebug() << data->getName();
    qDebug() << QString::fromStdString(data->getURL().full());

    auto status = data->open();
    if (!status.isOK()) {
        qDebug() << "Open failed:" << QString::fromStdString(status.message());
    }


    osgEarth::Style buildingStyle;
    buildingStyle.setName( "default" );

    osgEarth::ExtrusionSymbol* extrusion = buildingStyle.getOrCreate<osgEarth::ExtrusionSymbol>();
    extrusion->heightExpression() = osgEarth::NumericExpression( "3.5 * max( [story_ht_], 1 )" );
    extrusion->flatten() = true;
    extrusion->wallStyleName() = "building-wall";
    extrusion->roofStyleName() = "building-roof";

    osgEarth::PolygonSymbol* poly = buildingStyle.getOrCreate<osgEarth::PolygonSymbol>();
    poly->fill()->color() = osgEarth::Color::White;

    osgEarth::AltitudeSymbol* alt = buildingStyle.getOrCreate<osgEarth::AltitudeSymbol>();
    alt->clamping() = alt->CLAMP_TO_TERRAIN;
    alt->binding() = alt->BINDING_VERTEX;

    osgEarth::Style wallStyle;
    wallStyle.setName( "building-wall" );
    osgEarth::SkinSymbol* wallSkin = wallStyle.getOrCreate<osgEarth::SkinSymbol>();
    wallSkin->library() = "us_resources";
    wallSkin->addTag( "building" );
    wallSkin->randomSeed() = 1;

    osgEarth::Style roofStyle;
    roofStyle.setName( "building-roof" );
    osgEarth::SkinSymbol* roofSkin = roofStyle.getOrCreate<osgEarth::SkinSymbol>();
    roofSkin->library() = "us_resources";
    roofSkin->addTag( "rooftop" );
    roofSkin->randomSeed() = 1;
    roofSkin->isTiled() = true;

    osgEarth::StyleSheet* styleSheet = new osgEarth::StyleSheet();
    styleSheet->addStyle( buildingStyle );
    styleSheet->addStyle( wallStyle );
    styleSheet->addStyle( roofStyle );

    osgEarth::ResourceLibrary* reslib = new osgEarth::ResourceLibrary( "us_resources", RESOURCE_LIB_URL );
    styleSheet->addResourceLibrary( reslib );

    osgEarth::FeatureDisplayLayout layout;
    layout.tileSize() = 500;

    osgEarth::FeatureModelLayer* layer = new osgEarth::FeatureModelLayer();
    layer->setName("Buildings");
    layer->setFeatureSource(data);
    layer->setStyleSheet(styleSheet);
    layer->setLayout(layout);
    layer->setMaxVisibleRange(20000.0);

    m_map->addLayer(layer);
}

void OsgEarthRenderer2::addStreets() {
    osgEarth::OGRFeatureSource* data = new osgEarth::OGRFeatureSource();
    data->setURL(STREETS_URL);
    data->options().buildSpatialIndex() = true;

    osgEarth::ResampleFilterOptions resample;
    resample.minLength() = 0.0f;
    resample.maxLength() = 25.0f;
    data->options().filters().push_back( resample );

    osgEarth::Style style;
    style.setName( "streets" );

    osgEarth::LineSymbol* line = style.getOrCreate<osgEarth::LineSymbol>();
    line->stroke()->color() = osgEarth::Color(osgEarth::Color::Yellow, 0.5f);
    line->stroke()->width() = 7.5f;
    line->stroke()->widthUnits() = osgEarth::Units::METERS;

    osgEarth::AltitudeSymbol* alt = style.getOrCreate<osgEarth::AltitudeSymbol>();
    alt->clamping() = alt->CLAMP_TO_TERRAIN;

    osgEarth::RenderSymbol* render = style.getOrCreate<osgEarth::RenderSymbol>();
    render->depthOffset()->minBias()->set(6.6, osgEarth::Units::METERS);

    osgEarth::FeatureDisplayLayout layout;
    layout.tileSize() = 500;

    osgEarth::FeatureModelLayer* layer = new osgEarth::FeatureModelLayer();
    layer->setName("Streets");
    layer->setFeatureSource(data);
    layer->options().layout() = layout;
    layer->setStyleSheet(new osgEarth::StyleSheet());
    layer->getStyleSheet()->addStyle(style);
    layer->setMaxVisibleRange(5000.0f);

    m_map->addLayer(layer);
}

void OsgEarthRenderer2::addParks() {
    osgEarth::OGRFeatureSource* data = new osgEarth::OGRFeatureSource();
    data->setURL(PARKS_URL);
    data->options().buildSpatialIndex() = true;

    osgEarth::Style style;
    style.setName( "parks" );

    osgEarth::ModelSymbol* model = style.getOrCreate<osgEarth::ModelSymbol>();
    model->url()->setLiteral(TREE_MODEL_URL);
    model->placement() = model->PLACEMENT_RANDOM;
    model->density() = 6000.0f;

    osgEarth::AltitudeSymbol* alt = style.getOrCreate<osgEarth::AltitudeSymbol>();
    alt->clamping() = alt->CLAMP_TO_TERRAIN;

    osgEarth::RenderSymbol* render = style.getOrCreate<osgEarth::RenderSymbol>();
    render->transparent() = true;
    render->minAlpha() = 0.15f;

    osgEarth::FeatureDisplayLayout layout;
    layout.tileSize() = 650;
    layout.addLevel(osgEarth::FeatureLevel(0.0f, 2000.0f, "parks"));

    osgEarth::FeatureModelLayer* layer = new osgEarth::FeatureModelLayer();
    layer->setFeatureSource(data);
    layer->options().layout() = layout;
    layer->setStyleSheet(new osgEarth::StyleSheet());
    layer->getStyleSheet()->addStyle(style);

    m_map->addLayer(layer);
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
    QSize size = item->size().toSize();
    if (size.isEmpty()) {
        qWarning() << "Empty size in synchronize";
        return;
    }
    osg::Camera* camera = m_viewer->getCamera();
    if (camera && camera->getViewport()) {
        camera->getViewport()->setViewport(0, 0, size.width(), size.height());
        camera->setProjectionMatrixAsPerspective(30.0, size.width() / (double)size.height(), 1.0, 10000.0);
    }
    qDebug() << "Sync size:" << size;
}

void OsgEarthRenderer2::handleMouseEvent(QMouseEvent* event) {
    qDebug() << "Inside Mouse Handler";
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

void OsgEarthRenderer2::handleWheelEvent(QWheelEvent* event) {
    qDebug() << "Inside Wheel Handler";

    auto queue = m_viewer->getEventQueue();
    if (queue) {
        float delta = event->angleDelta().y() / 120.0f;
        queue->mouseScroll((delta > 0) ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
}
