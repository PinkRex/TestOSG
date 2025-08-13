import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
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

    Button {
        text: "Focus Boston"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onClicked: earthItem.focusBoston()
    }

    Button {
        id: toggleButton
        text: toggleButton.checked ? "2D Mode" : "3D Mode"
        checkable: true
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onCheckedChanged: {
            earthItem.setMode2D(checked);
        }
    }

    Button {
        text: "Toggle Buildings"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        onClicked: earthItem.toggleBuildings()
    }
}
