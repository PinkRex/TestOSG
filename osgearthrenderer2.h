#pragma once

#include <osgViewer/Viewer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <QQuickFramebufferObject>
#include <osgEarth/FeatureModelLayer>

class OsgEarthRenderer2 : public QQuickFramebufferObject::Renderer {
public:
    OsgEarthRenderer2();
    ~OsgEarthRenderer2();

    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item) override;

    void handleMouseEvent(QMouseEvent* event);
    void handleWheelEvent(QWheelEvent* event);
    // void handleKeyEvent(QKeyEvent* event);

public slots:
    void focusBoston();
    void setMode2D(bool enable2D);
    void toggleBuildings();

private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osgEarth::Map> m_map;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;
    osg::ref_ptr<osgEarth::FeatureModelLayer> m_buildingsLayer = nullptr;
    bool _is2D = false;
    void initOsgEarthScene();
    void addImagery();
    void addElevation();
    void addBuildings();
    void removeBuildings();
    void addStreets();
    void addParks();
};
