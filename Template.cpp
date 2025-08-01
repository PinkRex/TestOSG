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

int main(int argc, char** argv)
{
    osgEarth::initialize();
    osg::ArgumentParser arguments(&argc,argv);

    // create the map.
    osg::ref_ptr<Map> map = new Map();

    addImagery( map.get() );
    addElevation( map.get() );

    // initialize a viewer:
    osgViewer::Viewer viewer(arguments);

    EarthManipulator* manip = new EarthManipulator();
    viewer.setCameraManipulator( manip );

    osg::Group* root = new osg::Group();
    viewer.setSceneData( root );

    // make the map scene graph:
    MapNode* mapNode = new MapNode(map.get());
    root->addChild( mapNode );

    // zoom to a good startup position
    manip->setViewpoint( Viewpoint(
        "Home",
        -71.0763, 42.34425, 0,   // longitude, latitude, altitude
         24.261, -21.6, 3450.0), // heading, pitch, range
         5.0 );                    // duration

    // This will mitigate near clip plane issues if you zoom in close to the ground:
    LogarithmicDepthBuffer buf;
    buf.install( viewer.getCamera() );

    return viewer.run();
}

void addImagery(Map* map)
{
    // add a TMS imagery layer:
    TMSImageLayer* layer = new TMSImageLayer();
    layer->setURL(IMAGERY_URL);
    map->addLayer(layer);
}


void addElevation(Map* map)
{
    // add a TMS elevation layer:
    TMSElevationLayer* layer = new TMSElevationLayer();
    layer->setURL(ELEVATION_URL);
    map->addLayer(layer);
}

