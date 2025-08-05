#pragma once
#include <QQuickFramebufferObject>
#include <osgearthrenderer2.h>

class OsgEarthItem2 : public QQuickFramebufferObject {
    Q_OBJECT
public:
    explicit OsgEarthItem2(QQuickItem* parent = nullptr);
    Renderer *createRenderer() const override;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    // void keyPressEvent(QKeyEvent* event) override;

private:
    OsgEarthRenderer2* m_renderer = nullptr;
};
