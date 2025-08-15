#pragma once
#include <QQuickFramebufferObject>

#include <osgViewer/Viewer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/FeatureModelLayer>

class OsgEarthRenderer_3D : public QQuickFramebufferObject::Renderer {
public:
    OsgEarthRenderer_3D();
    ~OsgEarthRenderer_3D();

    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item) override;

    void handleMouseEvent(QMouseEvent* event);
    void handleMousePressEvent(QMouseEvent *event);
    void handleWheelEvent(QWheelEvent* event);
    // void handleKeyEvent(QKeyEvent* event);

public slots:
    void focusBoston();
    void setMode2D(bool enable2D);
    void toggleBuildings();
    void toggleStreets();
    void toggleParks();

private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osgEarth::Map> m_map;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;
    osg::ref_ptr<osgEarth::FeatureModelLayer> m_buildingsLayer = nullptr;
    osg::ref_ptr<osgEarth::FeatureModelLayer> m_streetsLayer = nullptr;
    osg::ref_ptr<osgEarth::FeatureModelLayer> m_parksLayer = nullptr;
    bool _is2D = false;
    bool _isBusy = false;
    void initOsgEarthScene();
    void addImagery();
    void addElevation();
    void addBuildings();
    void removeBuildings();
    void addStreets();
    void removeStreets();
    void addParks();
    void removeParks();
};
