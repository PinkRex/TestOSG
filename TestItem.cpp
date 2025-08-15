// #include <QDebug>

// #include "3D_Handler/OsgEarthItem_3D.h"
// #include "3D_Handler/OsgEarthRenderer_3D.h"

// OsgEarthItem_3D::OsgEarthItem_3D(QQuickItem* parent) : QQuickFramebufferObject(parent)
// {
//     setAcceptedMouseButtons(Qt::AllButtons);
//     setAcceptHoverEvents(true);
//     setFlag(ItemHasContents, true);
//     setFocus(true);
//     setFocusPolicy(Qt::StrongFocus);
//     setMirrorVertically(true);
// }

// QQuickFramebufferObject::Renderer* OsgEarthItem_3D::createRenderer() const {
//     auto r = new OsgEarthRenderer_3D();
//     const_cast<OsgEarthItem_3D*>(this)->m_renderer = r;
//     return r;
// }
