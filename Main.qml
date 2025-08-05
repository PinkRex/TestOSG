import QtQuick 2.15
import QtQuick.Window 2.15
import OsgEarth2 1.0

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("osgEarth in QML")

    OsgEarthItem2 {
        id: earthItem
        anchors.fill: parent
        focus: true
    }
}
