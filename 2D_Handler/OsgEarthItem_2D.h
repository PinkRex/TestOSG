#pragma once
#include <QQuickFramebufferObject>

#include "2D_Handler/OsgEarthRenderer_2D.h"

class OsgEarthItem_2D : public QQuickFramebufferObject {
    Q_OBJECT
public:
    explicit OsgEarthItem_2D(QQuickItem* parent = nullptr);
    Renderer *createRenderer() const override;
    Q_INVOKABLE void focusHanoi();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    OsgEarthRenderer_2D* m_renderer_2D = nullptr;
};
