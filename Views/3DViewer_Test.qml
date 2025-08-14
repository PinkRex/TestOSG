import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls

import OsgEarth2 1.0
Item {
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

    Column {
        id: buttonColumn
        spacing: 5
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10

        Button {
            text: "Back to Mode Chooser"
            onClicked: root.closeExternal()
        }

        Button {
            text: "Toggle Buildings"
            onClicked: earthItem.toggleBuildings()
        }

        Button {
            text: "Toggle Streets"
            onClicked: earthItem.toggleStreets()
        }

        Button {
            text: "Toggle Parks"
            onClicked: earthItem.toggleParks()
        }
    }
}
