#include <QDebug>

#include "OsgEarthItem_3D.h"
#include "3D_Handler/OsgEarthRenderer_3D.h"

OsgEarthItem_3D::OsgEarthItem_3D(QQuickItem* parent) : QQuickFramebufferObject(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents, true);
    setFocus(true);
    setFocusPolicy(Qt::StrongFocus);
    setMirrorVertically(true);
}

QQuickFramebufferObject::Renderer* OsgEarthItem_3D::createRenderer() const {
    auto r = new OsgEarthRenderer_3D();
    const_cast<OsgEarthItem_3D*>(this)->m_renderer = r;
    return r;
}

void OsgEarthItem_3D::mousePressEvent(QMouseEvent* event) {
    if (m_renderer) {
        m_renderer->handleMouseEvent(event);
        m_renderer->handleMousePressEvent(event);
    }
    QQuickFramebufferObject::mousePressEvent(event);
}

void OsgEarthItem_3D::mouseMoveEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseMoveEvent(event);
}

void OsgEarthItem_3D::mouseReleaseEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseReleaseEvent(event);
}

void OsgEarthItem_3D::wheelEvent(QWheelEvent* event) {
    if (m_renderer) m_renderer->handleWheelEvent(event);
    QQuickFramebufferObject::wheelEvent(event);
}

void OsgEarthItem_3D::focusBoston() {
    if (m_renderer) m_renderer->focusBoston();
}

void OsgEarthItem_3D::setMode2D(bool enable) {
    if (m_renderer) m_renderer->setMode2D(enable);
}

void OsgEarthItem_3D::toggleBuildings() {
    if (m_renderer) m_renderer->toggleBuildings();
}

void OsgEarthItem_3D::toggleStreets() {
    if (m_renderer) m_renderer->toggleStreets();
}

void OsgEarthItem_3D::toggleParks() {
    if (m_renderer) m_renderer->toggleParks();
}
