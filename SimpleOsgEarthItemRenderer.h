#pragma once

#include <QQuickFramebufferObject>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>
#include <osgEarth/EarthManipulator>
#include <osgGA/EventQueue>

class QTimer;

class SimpleOsgEarthItem;

class SimpleOsgEarthItemRenderer : public QQuickFramebufferObject::Renderer
{
public:
    explicit SimpleOsgEarthItemRenderer(const SimpleOsgEarthItem* item);
    ~SimpleOsgEarthItemRenderer() override;

    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
    void synchronize(QQuickFramebufferObject* item) override;

private:
    void initOsg();
    void initOsgEarth(const std::string& earthFilePath);

    const SimpleOsgEarthItem* m_item;
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_window;
    osg::ref_ptr<osg::Group> m_root;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> m_manipulator;
    bool m_initialized;
};

class SimpleOsgEarthItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString earthFilePath READ earthFilePath WRITE setEarthFilePath NOTIFY earthFilePathChanged)

public:
    explicit SimpleOsgEarthItem(QQuickItem* parent = nullptr);
    ~SimpleOsgEarthItem() override;

    Renderer* createRenderer() const override;
    osgGA::EventQueue* getEventQueue() const { return m_eventQueue.get(); }

    QString earthFilePath() const { return m_earthFilePath; }
    void setEarthFilePath(const QString& path);

protected:
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

signals:
    void earthFilePathChanged();

private:
    osg::ref_ptr<osgGA::EventQueue> m_eventQueue;
    QTimer* m_updateTimer;
    QString m_earthFilePath;
};
