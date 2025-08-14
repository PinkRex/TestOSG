import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Item {
    Rectangle {
        id: glassPanel
        width: root.width * 0.7
        height: root.height * 0.7
        radius: 20
        scale: 0.3
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 100
        border.color: "#80ffffff"
        border.width: 2
        color: "transparent"

        Rectangle {
            anchors.fill: parent
            radius: 20
            color: "#FFFFFFFF"
            opacity: 0.25
        }

        FastBlur {
            anchors.fill: parent
            source: sourceBackgroundImage
            radius: 30
            transparentBorder: true
        }

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: glassPanel.width
                height: glassPanel.height
                radius: glassPanel.radius
                color: "white"
            }
        }

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
                id: titleText
                text: "GIS Data Preprocess Module"
                font.pixelSize: 48
                font.bold: true
                font.family: "Roboto"
                color: "#00FFFF"

                layer.enabled: true
                layer.effect: Glow {
                    color: "#80FFFFFF"
                    radius: 10
                    samples: 24
                }
            }

            Item {
                width: titleText.width
                height: 30

                Text {
                    text: "VDT Program 2025"
                    font.pixelSize: 24
                    font.bold: false
                    font.family: "Roboto"
                    color: "#00FFFF"
                    opacity: 0.8
                    anchors.centerIn: parent
                    layer.enabled: true
                    layer.effect: Glow {
                        color: "#80FFFFFF"
                        radius: 8
                        samples: 20
                    }
                }
            }
        }

        Component.onCompleted: growAnimation.start()
        ParallelAnimation {
            id: growAnimation
            NumberAnimation { target: glassPanel; property: "scale"; from: 0.3; to: 1.0; duration: 600; easing.type: Easing.InOutQuad }
            NumberAnimation { target: glassPanel; property: "anchors.verticalCenterOffset"; from: 100; to: 0; duration: 600; easing.type: Easing.InOutQuad }
        }
    }
}
