import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Universal 2.2
import cpp.backend 1.0

ApplicationWindow {
    id: mainWindow
    visible: true

    property BackEnd backend: BackEnd {

    }
    width: 480
    height: 800


    header: ToolBar {
        width: parent.width
        RowLayout {
            spacing: 20
            anchors.fill: parent

            Label {
                id: titleLabel
                text: "   WiFi Car Control"
                font.pixelSize: 16
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }



    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Pane {
            id: pane1
            height: 80
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            ColumnLayout {
                anchors.fill: parent
                height: 50

                RowLayout {
                    width: parent.width
                    height: parent.height / 2

                    Label {
                        id: ipLabel
                        text: qsTr("IP:")
                        height: parent.height
                        font.pixelSize: 16
                    }

                    TextField {
                        id: ipInput
                        width: 200
                        height: parent.height
                        text: qsTr("192.168.10.1")
                        font.pixelSize: 16
                        anchors.right: parent.right

                        onTextChanged: backend.serverAddress = text
                    }
                }

                RowLayout {
                    width: parent.width
                    height: parent.height / 2

                    Label {
                        id: portLabel
                        text: qsTr("Port:")
                        height: parent.height
                        font.pixelSize: 16
                    }

                    TextField {
                        id: portInput
                        x: 50
                        width: 200
                        text: qsTr("4210")
                        height: parent.height
                        font.pixelSize: 16
                        anchors.right: parent.right

                        onTextChanged: backend.serverPort = text
                    }

                }


            }
        }

        Pane {
            id: pane2
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.top: pane1.bottom
            anchors.topMargin: 40
            anchors.bottom: pane3.top
            anchors.bottomMargin: 40

            Grid {
                id: grid
                anchors.fill: parent
                rows: 3
                columns: 3

                Button {
                    id: buttonTopLeft
                    text: qsTr("↖")

                    onClicked: {
                        backend.leftDialValue = 512
                        leftSlider.value = 512
                        backend.rightDialValue = 1023
                        rightSlider.value = 1023
                    }
                }

                Button {
                    id: buttonTop
                    text: qsTr("↑")

                    onClicked: {
                        backend.leftDialValue = 1023
                        leftSlider.value = 1023
                        backend.rightDialValue = 1023
                        rightSlider.value = 1023
                    }
                }

                Button {
                    id: buttonTopRight
                    text: qsTr("↗")

                    onClicked: {
                        backend.leftDialValue = 1023
                        leftSlider.value = 1023
                        backend.rightDialValue = 512
                        rightSlider.value = 512
                    }
                }

                Button {
                    id: buttonLeft
                    text: qsTr("←")

                    onClicked: {
                        backend.leftDialValue = 0
                        leftSlider.value = 0
                        backend.rightDialValue = 1023
                        rightSlider.value = 1023
                    }
                }

                Button {
                    text: qsTr("Stop")

                    onClicked: {
                        backend.leftDialValue = 0
                        leftSlider.value = 0
                        backend.rightDialValue = 0
                        rightSlider.value = 0
                    }
                }

                Button {
                    id: buttonRight
                    text: qsTr("→")

                    onClicked: {
                        backend.leftDialValue = 1023
                        leftSlider.value = 1023
                        backend.rightDialValue = 0
                        rightSlider.value = 0
                    }
                }

                Button {
                    id: buttonBottomLeft
                    text: qsTr("↙")

                    onClicked: {
                        backend.leftDialValue = -512
                        leftSlider.value = -512
                        backend.rightDialValue = -1023
                        rightSlider.value = -1023
                    }
                }

                Button {
                    id: buttonBottom
                    text: qsTr("↓")

                    onClicked: {
                        backend.leftDialValue = -1023
                        leftSlider.value = -1023
                        backend.rightDialValue = -1023
                        rightSlider.value = -1023
                    }
                }

                Button {
                    id: buttonBottomRight
                    text: qsTr("↘")

                    onClicked: {
                        backend.leftDialValue = -1023
                        leftSlider.value = -1023
                        backend.rightDialValue = -512
                        rightSlider.value = -512
                    }
                }


            }


        }

        Pane {
            id: pane3
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            RowLayout {
                anchors.fill: parent

                Pane {
                    width: parent.width / 2
                    height: parent.height
                    anchors.left: parent.left
                    anchors.top: parent.top

                    Slider {
                        id: leftSlider
                        to: 1023
                        from: -1023
                        orientation: Qt.Vertical

                        onValueChanged: {
                            backend.leftDialValue = value
                        }
                    }
                }

                Pane {
                    width: parent.width / 2
                    height: parent.height
                    anchors.right: parent.right
                    anchors.top: parent.top

                    Slider {
                        id: rightSlider
                        to: 1023
                        from: -1023
                        orientation: Qt.Vertical

                        onValueChanged: {
                            backend.rightDialValue = value
                        }
                    }
                }
            }


        }
    }












}
