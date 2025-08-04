#pragma once
#include <QQuickFramebufferObject>

class OsgEarthItem2 : public QQuickFramebufferObject {
    Q_OBJECT
public:
    Renderer *createRenderer() const override;
};
