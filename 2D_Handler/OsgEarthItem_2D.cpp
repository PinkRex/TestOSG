#include "OsgEarthItem_2D.h"
#include "OsgEarthRenderer_2D.h"

OsgEarthItem_2D::OsgEarthItem_2D(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents, true);
    setFocus(true);
    setFocusPolicy(Qt::StrongFocus);
    setMirrorVertically(true);
}

QQuickFramebufferObject::Renderer *OsgEarthItem_2D::createRenderer() const {
    auto r = new OsgEarthRenderer_2D();
    const_cast<OsgEarthItem_2D*>(this)->m_renderer = r;
    return r;
}

void OsgEarthItem_2D::mousePressEvent(QMouseEvent* event) {
    if (m_renderer) {
        m_renderer->handleMouseEvent(event);
        m_renderer->handleMousePressEvent(event);
    }
    QQuickFramebufferObject::mousePressEvent(event);
}

void OsgEarthItem_2D::mouseMoveEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseMoveEvent(event);
}

void OsgEarthItem_2D::mouseReleaseEvent(QMouseEvent* event) {
    if (m_renderer) m_renderer->handleMouseEvent(event);
    QQuickFramebufferObject::mouseReleaseEvent(event);
}

void OsgEarthItem_2D::wheelEvent(QWheelEvent* event) {
    if (m_renderer) m_renderer->handleWheelEvent(event);
    QQuickFramebufferObject::wheelEvent(event);
}
