#pragma once
#include <QQuickFramebufferObject>

#include <osg/Camera>
#include <osgViewer/Viewer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/FeatureModelLayer>

#include "3D_Handler/OsgEarthRenderer_3D.h"

class OsgEarthRenderer_2D : public QQuickFramebufferObject::Renderer
{
public:
    OsgEarthRenderer_2D();
    ~OsgEarthRenderer_2D();

    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item) override;

    void handleMouseEvent(QMouseEvent* event);
    void handleMousePressEvent(QMouseEvent *event);
    void handleWheelEvent(QWheelEvent* event);

public slots:
    void focusHanoi();

private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osgEarth::Map> m_map;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;

    OsgEarthRenderer_3D* renderer_3D;

    void initOsgEarthScene();
    void addImagery();
};
