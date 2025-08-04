import QtQuick 2.15
import QtQuick.Window 2.15
import OsgEarth2 1.0

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("osgEarth in QML")

    // SimpleOsgEarthItem {
    //     id: earthItem
    //     anchors.fill: parent
    //     earthFile: "/home/canhdx/OSG_Test/Test.earth"
    // }

    OsgEarthItem2 {
        id: earthItem
        anchors.fill: parent
    }
}
