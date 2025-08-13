#pragma once
#include <QQuickFramebufferObject>
#include <osgearthrenderer2.h>

class OsgEarthItem2 : public QQuickFramebufferObject {
    Q_OBJECT
public:
    explicit OsgEarthItem2(QQuickItem* parent = nullptr);
    Renderer *createRenderer() const override;
    Q_INVOKABLE void focusBoston();
    Q_INVOKABLE void setMode2D(bool enable);
    Q_INVOKABLE void toggleBuildings();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    // void keyPressEvent(QKeyEvent* event) override;

private:
    OsgEarthRenderer2* m_renderer = nullptr;
};
