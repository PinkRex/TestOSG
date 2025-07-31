import QtQuick 2.15
import QtQuick.Window 2.15
import OsgEarth 1.0

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("osgEarth in QML")

    SimpleOsgEarthItemRenderer {
        id: earthItem
    //     anchors.fill: parent
    //     earthFilePath: "/home/canhdx/OSG_Test/Test.earth"
    }
}
