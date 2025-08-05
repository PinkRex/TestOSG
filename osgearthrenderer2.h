#pragma once

#include <osgViewer/Viewer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <QQuickFramebufferObject>

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

private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osgEarth::Map> m_map;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;
    void initOsgEarthScene();
    void addImagery();
    void addElevation();
    void addBuildings();
    void addStreets();
    void addParks();
};
