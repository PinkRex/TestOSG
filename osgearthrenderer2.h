#ifndef OSGEARTHRENDERER2_H
#define OSGEARTHRENDERER2_H

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <QQuickFramebufferObject>
#include <osgearthitem2.h>

class OsgEarthRenderer2 : public QQuickFramebufferObject::Renderer {

public:
    OsgEarthRenderer2(OsgEarthItem2* item);
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
    void render() override;

private:
    void loadEarthFile(const QString& path);
    OsgEarthItem2* m_item;
    osgViewer::Viewer m_viewer;
    osg::ref_ptr<osg::Node> m_root;
    osg::ref_ptr<osgGA::CameraManipulator> m_manip;
    bool m_initialized = false;
    bool m_firstTime = true;
    int frameCount = 0;
    QSizeF m_lastSize;
};

#endif // OSGEARTHRENDERER2_H
