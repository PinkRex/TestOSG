import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Qt5Compat.GraphicalEffects

import OsgEarth_3D 1.0
import "Views"

Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("osgEarth in QML")

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
