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
    auto renderer_2D = new OsgEarthRenderer_2D();
    const_cast<OsgEarthItem_2D*>(this)->m_renderer_2D = renderer_2D;
    return renderer_2D;
}

void OsgEarthItem_2D::mousePressEvent(QMouseEvent* event) {
    if (m_renderer_2D) {
        m_renderer_2D->handleMouseEvent(event);
        m_renderer_2D->handleMousePressEvent(event);
    }
    QQuickFramebufferObject::mousePressEvent(event);
}

void OsgEarthItem_2D::mouseMoveEvent(QMouseEvent* event) {
    if (m_renderer_2D) m_renderer_2D->handleMouseEvent(event);
    QQuickFramebufferObject::mouseMoveEvent(event);
}

void OsgEarthItem_2D::mouseReleaseEvent(QMouseEvent* event) {
    if (m_renderer_2D) m_renderer_2D->handleMouseEvent(event);
    QQuickFramebufferObject::mouseReleaseEvent(event);
}

void OsgEarthItem_2D::wheelEvent(QWheelEvent* event) {
    if (m_renderer_2D) m_renderer_2D->handleWheelEvent(event);
    QQuickFramebufferObject::wheelEvent(event);
}

void OsgEarthItem_2D::focusHanoi() {
    if (m_renderer_2D) m_renderer_2D->focusHanoi();
}
