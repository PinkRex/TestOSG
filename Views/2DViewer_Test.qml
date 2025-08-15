import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls

import OsgEarth_2D 1.0

Item {
    OsgEarthItem_2D {
        id: earthItem
        anchors.fill: parent
        focus: true
    }

    Button {
        text: "Focus Ha Noi"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onClicked: earthItem.focusHanoi()
    }

    Button {
        text: "Back to Mode Chooser"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: root.closeExternal()
    }
}
