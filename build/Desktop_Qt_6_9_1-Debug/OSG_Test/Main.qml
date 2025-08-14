import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Qt5Compat.GraphicalEffects

import OsgEarth2 1.0
import "Views"

Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("osgEarth in QML")

    // OsgEarthItem2 {
    //     id: earthItem
    //     anchors.fill: parent
    //     focus: true
    // }

    // Button {
    //     text: "Focus Boston"
    //     anchors.bottom: parent.bottom
    //     anchors.right: parent.right
    //     onClicked: earthItem.focusBoston()
    // }

    // Button {
    //     id: toggleButton
    //     text: toggleButton.checked ? "2D Mode" : "3D Mode"
    //     checkable: true
    //     anchors.bottom: parent.bottom
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     onCheckedChanged: {
    //         earthItem.setMode2D(checked);
    //     }
    // }

    // Column {
    //     id: buttonColumn
    //     spacing: 5
    //     anchors.left: parent.left
    //     anchors.bottom: parent.bottom
    //     anchors.margins: 10

    //     Button {
    //         text: "Toggle Buildings"
    //         onClicked: earthItem.toggleBuildings()
    //     }

    //     Button {
    //         text: "Toggle Streets"
    //         onClicked: earthItem.toggleStreets()
    //     }

    //     Button {
    //         text: "Toggle Parks"
    //         onClicked: earthItem.toggleParks()
    //     }
    // }

    Image {
        id: sourceBackgroundImage
        source: "qrc:/images/background.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    SwipeView {
        id: mainSwipeView
        anchors.fill: parent
        currentIndex: 0
        orientation: "Vertical"

        StartUpUI {}
        ModeChooserUI {}
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "#000"
        opacity: 0
        visible: opacity > 0
        z: 998
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }

    Loader {
        id: fileLoader
        anchors.fill: parent
        asynchronous: true
        active: false
        visible: false
        z: 999
    }

    function openExternal(sourceFile) {
        overlay.opacity = 1
        timerLoadIn.sourceFile = sourceFile
        timerLoadIn.start()
    }

    function closeExternal() {
        overlay.opacity = 1
        timerLoadOut.start()
    }

    Timer {
        id: timerLoadIn
        interval: 500
        repeat: false
        property string sourceFile: ""
        onTriggered: {
            fileLoader.source = sourceFile
            fileLoader.active = true
            fileLoader.visible = true
        }
    }

    Timer {
        id: timerLoadOut
        interval: 800
        repeat: false
        onTriggered: {
            fileLoader.source = ""
            fileLoader.active = false
            fileLoader.visible = false
            overlay.opacity = 0
        }
    }
}
