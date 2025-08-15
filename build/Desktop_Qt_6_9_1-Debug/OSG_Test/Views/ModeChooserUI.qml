import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Item {
    Image {
        id: sourceLeftBtnImage
        source: "qrc:/images/leftBtn.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        opacity: 0
    }

    Image {
        id: sourceRightBtnImage
        source: "qrc:/images/rightBtn.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        opacity: 0
    }

    Row {
        anchors.fill: parent

        spacing: 0

        // Left half
        Rectangle {
            width: parent.width / 2
            height: parent.height
            color: "transparent"

            Rectangle {
                id: button1
                width: parent.width * 0.5
                height: parent.height * 0.25
                radius: 20
                color: "transparent"
                border.color: "#80FFFFFF"
                border.width: 2
                anchors.centerIn: parent

                Rectangle {
                    anchors.fill: parent
                    radius: 20
                    color: "#FFFFFFFF"
                    opacity: 0.25
                }

                FastBlur {
                    anchors.fill: parent
                    source: sourceLeftBtnImage
                    radius: 20
                    transparentBorder: true
                }

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: button1.width
                        height: button1.height
                        radius: button1.radius
                        color: "white"
                    }
                }

                Text {
                    text: "Function 1"
                    anchors.centerIn: parent
                    font.pixelSize: 32
                    font.bold: true
                    font.family: "Roboto"
                    color: "#FFFFFF"
                    opacity: 0.8
                    layer.enabled: true
                    layer.effect: Glow {
                        color: "#80FFFFFF"
                        radius: 8
                        samples: 20
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onEntered: button1.scale = 1.15
                    onExited:  button1.scale = 1.0
                    onClicked: root.openExternal("Views/2DViewer_Test.qml")
                }

                Behavior on scale {
                    NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
                }
            }
        }

        // Right half
        Rectangle {
            width: parent.width / 2
            height: parent.height
            color: "transparent"

            Rectangle {
                id: button2
                width: parent.width * 0.5
                height: parent.height * 0.25
                radius: 20
                color: "transparent"
                border.color: "#80FFFFFF"
                border.width: 2
                anchors.centerIn: parent

                Rectangle {
                    anchors.fill: parent
                    radius: 20
                    color: "#FFFFFFFF"
                    opacity: 0.25
                }

                FastBlur {
                    anchors.fill: parent
                    source: sourceRightBtnImage
                    radius: 20
                    transparentBorder: true
                }

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: button1.width
                        height: button1.height
                        radius: button1.radius
                        color: "white"
                    }
                }

                Text {
                    text: "Function 2"
                    anchors.centerIn: parent
                    font.pixelSize: 32
                    font.bold: true
                    font.family: "Roboto"
                    color: "#FFFFFF"
                    layer.enabled: true
                    layer.effect: Glow {
                        color: "#80FFFFFF"
                        radius: 8
                        samples: 20
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onEntered: button2.scale = 1.15
                    onExited:  button2.scale = 1.0
                    onClicked: root.openExternal("Views/3DViewer_Test.qml")
                }

                Behavior on scale {
                    NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
                }
            }
        }
    }
}
