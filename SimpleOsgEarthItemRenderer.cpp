#include "SimpleOsgEarthItemRenderer.h"
#include <osgDB/ReadFile>
#include <osgEarth/GLUtils>
#include <QtQuick/QQuickWindow>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QTimer>
#include <qsgsimpletexturenode.h>
#include <QDebug>

SimpleOsgEarthItemRenderer::SimpleOsgEarthItemRenderer(const SimpleOsgEarthItem* item)
    : m_item(item), m_initialized(false)
{
}

SimpleOsgEarthItemRenderer::~SimpleOsgEarthItemRenderer()
{
}

void SimpleOsgEarthItemRenderer::initOsg()
{
    osg::setNotifyLevel(osg::NotifySeverity::WARN);

    m_viewer = new osgViewer::Viewer();
    m_window = m_viewer->setUpViewerAsEmbeddedInWindow(0, 0, m_item->width(), m_item->height());
    m_viewer->getCamera()->setGraphicsContext(m_window.get());
    m_viewer->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_viewer->getCamera()->setViewport(new osg::Viewport(0, 0, m_item->width(), m_item->height()));
    m_viewer->getCamera()->setProjectionMatrixAsPerspective(60, m_item->width() / m_item->height(), 1.0, 10000.0);
    m_root = new osg::Group();
    m_viewer->setSceneData(m_root);
}

void SimpleOsgEarthItemRenderer::initOsgEarth(const std::string& earthFilePath)
{
    osgEarth::GLUtils::setGlobalDefaults(m_viewer->getCamera()->getOrCreateStateSet());

    // Load file .earth
    osg::Node* loadedNode = osgDB::readNodeFile(earthFilePath);
    if (!loadedNode)
    {
        qDebug() << "Failed to load earth file:" << QString::fromStdString(earthFilePath);
        return;
    }

    // Tìm MapNode từ file .earth
    m_mapNode = osgEarth::MapNode::findMapNode(loadedNode);
    if (!m_mapNode)
    {
        qDebug() << "No MapNode found in earth file.";
        return;
    }

    m_root->addChild(loadedNode);
    m_manipulator = new osgEarth::Util::EarthManipulator();
    m_viewer->setCameraManipulator(m_manipulator);
}

QOpenGLFramebufferObject* SimpleOsgEarthItemRenderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}

void SimpleOsgEarthItemRenderer::render()
{
    if (m_viewer)
    {
        GLint drawFboId = 0;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
        m_viewer->getCamera()->getGraphicsContext()->setDefaultFboId(drawFboId);
        m_viewer->frame();
    }
    // m_item->window()->resetOpenGLState();
}

void SimpleOsgEarthItemRenderer::synchronize(QQuickFramebufferObject* item)
{
    if (!m_initialized)
    {
        initOsg();
        initOsgEarth(qPrintable(dynamic_cast<const SimpleOsgEarthItem*>(item)->earthFilePath()));
        m_initialized = true;
    }

    osgGA::EventQueue::Events events;
    dynamic_cast<const SimpleOsgEarthItem*>(item)->getEventQueue()->takeEvents(events);
    m_viewer->getEventQueue()->appendEvents(events);
}

SimpleOsgEarthItem::SimpleOsgEarthItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , m_eventQueue(new osgGA::EventQueue(osgGA::GUIEventAdapter::Y_INCREASING_DOWNWARDS))
    , m_earthFilePath("/home/canhdx/OSG_Test/Test.earth") // Thay bằng đường dẫn thực tế
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SimpleOsgEarthItem::update);
    m_updateTimer->start(16); // ~60 FPS
}

SimpleOsgEarthItem::~SimpleOsgEarthItem()
{
}

QQuickFramebufferObject::Renderer* SimpleOsgEarthItem::createRenderer() const
{
    return new SimpleOsgEarthItemRenderer(this);
}

// QString SimpleOsgEarthItem::earthFilePath() const
// {
//     return m_earthFilePath;
// }

void SimpleOsgEarthItem::setEarthFilePath(const QString& path)
{
    if (m_earthFilePath != path)
    {
        m_earthFilePath = path;
        emit earthFilePathChanged();
    }
}

QSGNode* SimpleOsgEarthItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updateData)
{
    QSGNode* node = QQuickFramebufferObject::updatePaintNode(oldNode, updateData);
    if (node)
    {
        QSGSimpleTextureNode* textureNode = dynamic_cast<QSGSimpleTextureNode*>(node);
        textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
    }
    return node;
}

void SimpleOsgEarthItem::geometryChanged(const QRectF& newGeo, const QRectF& oldGeo)
{
    m_eventQueue->windowResize(newGeo.x(), newGeo.y(), newGeo.width(), newGeo.height());
    QQuickFramebufferObject::geometryChange(newGeo, oldGeo);
}

void SimpleOsgEarthItem::mousePressEvent(QMouseEvent* event)
{
    m_eventQueue->mouseButtonPress(event->x(), event->y(), event->button() == Qt::LeftButton ? 1 : event->button() == Qt::RightButton ? 3 : 2);
    update();
}

void SimpleOsgEarthItem::mouseMoveEvent(QMouseEvent* event)
{
    m_eventQueue->mouseMotion(event->x(), event->y());
    update();
}

void SimpleOsgEarthItem::mouseReleaseEvent(QMouseEvent* event)
{
    m_eventQueue->mouseButtonRelease(event->x(), event->y(), event->button() == Qt::LeftButton ? 1 : event->button() == Qt::RightButton ? 3 : 2);
    update();
}

void SimpleOsgEarthItem::wheelEvent(QWheelEvent* event)
{
    // m_eventQueue->mouseScroll(event->delta() < 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
    // update();
}
