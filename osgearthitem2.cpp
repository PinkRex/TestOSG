#include "osgearthitem2.h"
#include "osgearthrenderer2.h"
#include <QDebug>

OsgEarthItem2::OsgEarthItem2(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents, true);
    setFocus(true);
    setFocusPolicy(Qt::StrongFocus);

    qDebug() << "[OsgEarthItem2] Constructor called, mouse accepted buttons ="
             << acceptedMouseButtons();
}

QQuickFramebufferObject::Renderer* OsgEarthItem2::createRenderer() const {
    // return new OsgEarthRenderer2();
    auto r = new OsgEarthRenderer2();
    const_cast<OsgEarthItem2*>(this)->m_renderer = r;
    return r;
}

void OsgEarthItem2::mousePressEvent(QMouseEvent* event) {
    qDebug() << "Mouse Pressed";
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mousePressEvent(event);
}

void OsgEarthItem2::mouseMoveEvent(QMouseEvent* event) {
    qDebug() << "Mouse Moved";
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseMoveEvent(event);
}

void OsgEarthItem2::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "Mouse Released";
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseReleaseEvent(event);
}

void OsgEarthItem2::wheelEvent(QWheelEvent* event) {
    qDebug() << "Wheel Used";
    if (m_renderer) m_renderer->handleWheelEvent(event);
    QQuickFramebufferObject::wheelEvent(event);
}
