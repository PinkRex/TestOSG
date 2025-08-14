import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import OsgEarth2 1.0

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
        source: "qrc:/images/leftBg.jpg"
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

}
