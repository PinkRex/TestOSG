#include "osgearthitem2.h"
#include "osgearthrenderer2.h"

QQuickFramebufferObject::Renderer* OsgEarthItem2::createRenderer() const {
    return new OsgEarthRenderer2();
}
