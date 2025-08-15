#pragma once
#include <QQuickFramebufferObject>
#include <OsgEarthRenderer_3D.h>

class OsgEarthItem_3D : public QQuickFramebufferObject {
    Q_OBJECT
public:
    explicit OsgEarthItem_3D(QQuickItem* parent = nullptr);
    Renderer *createRenderer() const override;
    Q_INVOKABLE void focusBoston();
    Q_INVOKABLE void setMode2D(bool enable);
    Q_INVOKABLE void toggleBuildings();
    Q_INVOKABLE void toggleStreets();
    Q_INVOKABLE void toggleParks();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    // void keyPressEvent(QKeyEvent* event) override;

private:
    OsgEarthRenderer_3D* m_renderer = nullptr;
};
