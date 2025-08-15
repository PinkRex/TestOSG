#include "OsgEarthRenderer_3D.h"
#include <osgEarth/EarthManipulator>
#include <osgEarth/LogarithmicDepthBuffer>
#include <osgEarth/ImageLayer>
#include <osgEarth/TMS>
#include <osgEarth/FeatureModelLayer>
#include <osgEarth/OGRFeatureSource>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/TerrainEngineNode>

#include <QOpenGLFramebufferObject>
#include <QTimer>

#define IMAGERY_URL      "http://readymap.org/readymap/tiles/1.0.0/22/"
#define ELEVATION_URL    "http://readymap.org/readymap/tiles/1.0.0/116/"
#define BUILDINGS_URL    "/home/canhdx/TestOSG/data/boston_buildings_utm19.shp"
#define RESOURCE_LIB_URL "/home/canhdx/TestOSG/data/catalog.xml"
#define STREETS_URL      "/home/canhdx/TestOSG/data/boston-scl-utm19n-meters.shp"
#define PARKS_URL        "/home/canhdx/TestOSG/data/boston-parks.shp"
#define TREE_MODEL_URL   "/home/canhdx/TestOSG/data/tree.osg"

OsgEarthRenderer_3D::OsgEarthRenderer_3D() {
    osgEarth::initialize();
    m_viewer = new osgViewer::Viewer;
    m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    m_viewer->getEventHandlers().clear();
    m_viewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
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

OsgEarthRenderer_3D::~OsgEarthRenderer_3D() {}

void OsgEarthRenderer_3D::initOsgEarthScene() {
    m_map = new osgEarth::Map;
    addImagery();
    addElevation();
    addBuildings();
    addStreets();
    addParks();

    _isBusy = false;
    m_mapNode = new osgEarth::MapNode(m_map.get());
    osg::Group* root = new osg::Group;
    root->addChild(m_mapNode.get());

    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (manip) {
        manip->setViewpoint(osgEarth::Viewpoint("Home", -71.0763, 42.34425, 0, 24.261, -21.6, 3450.0), 5.0);
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

void OsgEarthRenderer_3D::addImagery() {
    auto* layer = new osgEarth::TMSImageLayer;
    layer->setURL(IMAGERY_URL);
    m_map->addLayer(layer);
}

void OsgEarthRenderer_3D::addElevation() {
    auto* layer = new osgEarth::TMSElevationLayer;
    layer->setURL(ELEVATION_URL);
    m_map->addLayer(layer);
}

void OsgEarthRenderer_3D::addBuildings() {
    if (m_buildingsLayer) return;
    osgEarth::OGRFeatureSource* data = new osgEarth::OGRFeatureSource();
    data->setName("buildings-data");
    data->setURL(BUILDINGS_URL);

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

    osgEarth::Style wallStyle;if (m_buildingsLayer) return;
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

    m_buildingsLayer = new osgEarth::FeatureModelLayer();
    m_buildingsLayer->setName("Buildings");
    m_buildingsLayer->setFeatureSource(data);
    m_buildingsLayer->setStyleSheet(styleSheet);
    m_buildingsLayer->setLayout(layout);
    m_buildingsLayer->setMaxVisibleRange(20000.0);

    m_map->addLayer(m_buildingsLayer.get());
}

void OsgEarthRenderer_3D::removeBuildings() {
    if (!m_buildingsLayer) return;
    m_map->removeLayer(m_buildingsLayer.get());
    m_buildingsLayer = nullptr;
}

void OsgEarthRenderer_3D::addStreets() {
    if (m_streetsLayer) return;
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

    m_streetsLayer = new osgEarth::FeatureModelLayer();
    m_streetsLayer->setName("Streets");
    m_streetsLayer->setFeatureSource(data);
    m_streetsLayer->options().layout() = layout;
    m_streetsLayer->setStyleSheet(new osgEarth::StyleSheet());
    m_streetsLayer->getStyleSheet()->addStyle(style);
    m_streetsLayer->setMaxVisibleRange(5000.0f);

    m_map->addLayer(m_streetsLayer.get());
}

void OsgEarthRenderer_3D::removeStreets() {
    if (!m_streetsLayer) return;
    m_map->removeLayer(m_streetsLayer.get());
    m_streetsLayer = nullptr;
}

void OsgEarthRenderer_3D::addParks() {
    if (m_parksLayer) return;
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

    m_parksLayer = new osgEarth::FeatureModelLayer();
    m_parksLayer->setFeatureSource(data);
    m_parksLayer->options().layout() = layout;
    m_parksLayer->setStyleSheet(new osgEarth::StyleSheet());
    m_parksLayer->getStyleSheet()->addStyle(style);

    m_map->addLayer(m_parksLayer.get());
}

void OsgEarthRenderer_3D::removeParks() {
    if (!m_parksLayer) return;
    m_map->removeLayer(m_parksLayer.get());
    m_parksLayer = nullptr;
}

QOpenGLFramebufferObject *OsgEarthRenderer_3D::createFramebufferObject(const QSize &size) {
    if (size.width() <= 0 || size.height() <= 0) {
        qWarning() << "Invalid FBO size:" << size;
        return nullptr;
    }
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    return new QOpenGLFramebufferObject(size, format);
}

void OsgEarthRenderer_3D::render() {
    m_viewer->frame();
    update();
}

void OsgEarthRenderer_3D::synchronize(QQuickFramebufferObject *item) {
    QSize size = item->size().toSize();
    if (size.isEmpty()) {
        qWarning() << "Empty size in synchronize";
        return;
    }
    osg::Camera* camera = m_viewer->getCamera();
    if (camera && camera->getViewport()) {
        camera->getViewport()->setViewport(0, 0, size.width(), size.height());
        camera->setProjectionMatrixAsPerspective(30.0, size.width() / (double)size.height(), 1.0, 10000.0);

        camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
        camera->setRenderOrder(osg::Camera::POST_RENDER);

        camera->setViewMatrix(camera->getViewMatrix());
    }
}

void OsgEarthRenderer_3D::handleMouseEvent(QMouseEvent* event) {
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

void OsgEarthRenderer_3D::handleMousePressEvent(QMouseEvent *event) {
    if (!m_mapNode) return;
    osgViewer::View* view = m_viewer.get();
    if (!view) return;

    osg::Viewport* vp = view->getCamera()->getViewport();
    if (!vp) return;

    int fboW = this->framebufferObject()->width();
    int fboH = this->framebufferObject()->height();

    double osgX = event->pos().x() * (vp->width() / (double)fboW);
    double osgY = (fboH - event->pos().y()) * (vp->height() / (double)fboH);

    osgEarth::GeoPoint geo;
    auto hit = m_mapNode->getGeoPointUnderMouse(view, osgX, osgY);

    if (hit.isValid())
    {
        double lon, lat;
        // geo.(lon, lat);
        qDebug() << "Lat:" << lat << "Lon:" << lon;
    }
    else
    {
        qDebug() << "No hit";
    }
}

void OsgEarthRenderer_3D::handleWheelEvent(QWheelEvent* event) {
    auto queue = m_viewer->getEventQueue();
    if (!queue) return;

    float delta = event->angleDelta().y() / 120.0f;
    queue->mouseScroll((delta > 0) ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
}

void OsgEarthRenderer_3D::focusBoston() {
    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (manip) {
        manip->setViewpoint(osgEarth::Viewpoint(
                                "Boston",   // label
                                -71.0763,   // longitude
                                42.34425,   // latitude
                                0,          // altitude (m)
                                0,          // heading (deg)
                                -45.0,      // pitch (deg)
                                5000.0      // range (m)
                                ), 3.0);    // 3s animation
    }
}

void OsgEarthRenderer_3D::setMode2D(bool enable2D) {
    if (_is2D == enable2D) return;
    _is2D = enable2D;

    auto manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(m_viewer->getCameraManipulator());
    if (!manip) return;

    if (_is2D) {
        manip->setViewpoint(osgEarth::Viewpoint(
                                "2DView",   // label
                                -71.0763,   // longitude
                                42.34425,   // latitude
                                0,          // altitude (m)
                                0,          // heading (deg)
                                -90.0,      // pitch (deg)
                                5000.0      // range (m)
                                ), 3.0);    // 3s animation
    } else {
        manip->setViewpoint(osgEarth::Viewpoint(
                                "3DView",
                                -71.0763, 42.34425, 0,
                                24.261, -21.6, 3450.0
                                ), 2.0);
    }
}

void OsgEarthRenderer_3D::toggleBuildings() {
    if (_isBusy) return;
    _isBusy = true;

    if (m_buildingsLayer) {
        removeBuildings();
        QTimer::singleShot(50, [this]() {
            _isBusy = false;
        });
    } else {
        QTimer::singleShot(0, [this]() {
            addBuildings();
            _isBusy = false;
        });
    }

    _isBusy = false;
}

void OsgEarthRenderer_3D::toggleStreets() {
    if (_isBusy) return;
    _isBusy = true;

    if (m_streetsLayer) {
        removeStreets();
        QTimer::singleShot(50, [this]() {
            _isBusy = false;
        });
    } else {
        QTimer::singleShot(0, [this]() {
            addStreets();
            _isBusy = false;
        });
    }

    _isBusy = false;
}

void OsgEarthRenderer_3D::toggleParks() {
    if (_isBusy) return;

    _isBusy = true;

    if (m_parksLayer) {
        removeParks();
        QTimer::singleShot(50, [this]() {
            _isBusy = false;
        });
    } else {
        QTimer::singleShot(0, [this]() {
            addParks();
            _isBusy = false;
        });
    }

    _isBusy = false;
}
