#include "osgearthitem2.h"
#include "osgearthrenderer2.h"
#include <QDebug>

OsgEarthItem2::OsgEarthItem2(QQuickItem* parent) : QQuickFramebufferObject(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents, true);
    setFocus(true);
    setFocusPolicy(Qt::StrongFocus);
    setMirrorVertically(true);
}

QQuickFramebufferObject::Renderer* OsgEarthItem2::createRenderer() const {
    auto r = new OsgEarthRenderer2();
    const_cast<OsgEarthItem2*>(this)->m_renderer = r;
    return r;
}

void OsgEarthItem2::mousePressEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mousePressEvent(event);
}

void OsgEarthItem2::mouseMoveEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseMoveEvent(event);
}

void OsgEarthItem2::mouseReleaseEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseReleaseEvent(event);
}

void OsgEarthItem2::wheelEvent(QWheelEvent* event) {
    if (m_renderer) m_renderer->handleWheelEvent(event);
    QQuickFramebufferObject::wheelEvent(event);
}

void OsgEarthItem2::focusBoston() {
    if (m_renderer) m_renderer->focusBoston();
}

void OsgEarthItem2::setMode2D(bool enable) {
    if (m_renderer) m_renderer->setMode2D(enable);
}

void OsgEarthItem2::toggleBuildings() {
    if (m_renderer) m_renderer->toggleBuildings();
}
