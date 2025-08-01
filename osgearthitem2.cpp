#include "osgearthitem2.h"

#include <osg/Notify>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>

#include <osgEarth/MapNode>
#include <osgEarth/ImageLayer>
#include <osgEarth/TMS>

#include <osgEarth/ExampleResources>
#include <osgEarth/EarthManipulator>
#include <osgEarth/LogarithmicDepthBuffer>

#include <osgEarth/FeatureModelLayer>
#include <osgEarth/OGRFeatureSource>

using namespace osgEarth;
using namespace osgEarth::Util;

#define IMAGERY_URL      "http://readymap.org/readymap/tiles/1.0.0/22/"
#define ELEVATION_URL    "http://readymap.org/readymap/tiles/1.0.0/116/"

// forward declarations.
void addImagery  (Map* map);
void addElevation(Map* map);

OsgEarthItem2::OsgEarthItem2() {

}
