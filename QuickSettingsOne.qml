import QtQuick 2.0
import QtQuick.Controls 2.12
import DriveMode 1.0 //ENUM режимы вождения

Item {
    //
    property int lastMode: -1
    property int currentMode: 0

    ListModel {
        id: listModeDrive

        ListElement {
            name: qsTr("Key points");
            driveMode: DriveMode.KEYPOINTS_MODE;
            imageSource: "qrc:/image/icon-way-keypoins.png"
        }
        ListElement {
            name: qsTr("Parallel");
            driveMode: DriveMode.PARALLEL_MODE;
            imageSource: "qrc:/image/icon-way-snake.png"
        }
        ListElement {
            name: qsTr("Spiral");
            driveMode: DriveMode.SPIRAL_MODE;
            imageSource: "qrc:/image/icon-way-spiral.png"
        }
        ListElement {
            name: qsTr("None");
            driveMode: DriveMode.NONE_MODE;
            imageSource: "qrc:/image/icon-no-image.png"
        }
        ListElement {
            name: qsTr("None");
            driveMode: DriveMode.NONE_MODE;
            imageSource: "qrc:/image/icon-no-image.png"
        }

    }

    GridView {
        id: gridViewMode

        anchors.fill: parent
        anchors.leftMargin: 60
        anchors.topMargin: 60

        model: listModeDrive

        cellWidth: 240
        cellHeight: 220

        //clip: true // обрезка за пределами окна
        focus: true

        delegate: Rectangle {
            id: delegateGridViewMode

            width: 200
            height: 200
            radius: 15

            border.color: "black"//Qt.rgba(0.2, 0.2, 0.2, 1)
            border.width: 0

            color: Qt.rgba(0.5, 0.5, 0.5, 0.7)

            Image {
                width: height
                height: parent.height*0.75

                anchors.centerIn: parent
                source: imageSource
            }

            Text {
                color: "black"
                text: name
                font.pixelSize: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            MouseArea {
                anchors.fill: parent

                onPressed: {
                    delegateGridViewMode.color = Qt.rgba(0.4, 0.4, 0.4, 0.9)
                    delegateGridViewMode.border.width = 5
                }
                onReleased: {
                    currentMode = index
                    //if(driveMode == DriveMode.NONE_MODE){
                    console.log(driveMode)
                    //}

                    if(currentMode == lastMode) {
                        return
                    }

                    if(lastMode >= 0) {
                        gridViewMode.itemAtIndex(lastMode).color = Qt.rgba(0.5, 0.5, 0.5, 0.7)
                        gridViewMode.itemAtIndex(lastMode).border.width = 0
                    }
                    lastMode = currentMode
                    //delegateGridViewMode.color = Qt.rgba(0.4, 0.4, 0.4, 0.9)
                    //delegateGridViewMode.border.width = 2

                    //model.setDriveMode(driveMode)
                    modelView.setDriveModeFromQML(driveMode)
                }
                onCanceled: {
                    delegateGridViewMode.color = Qt.rgba(0.5, 0.5, 0.5, 0.7)
                    delegateGridViewMode.border.width = 0
                }
            }
        }
    }

    TextButton {
        id: btNextQiuckSettingsOne
        width: 80
        height: width

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 15
        }

        textButton: qsTr("Next")
        sizeTextButton: 20

        onReleasedButton: {
            if(lastMode < 0) {
                popupAttention.open()
                return
            }

            stackViewQuickSettings.push("QuickSettingsTwo.qml")
        }
    }

    TextButton {
        width: 80
        height: width

        anchors {
            right: btNextQiuckSettingsOne.left
            bottom: parent.bottom
            margins: 15
        }

        textButton: qsTr("Back")
        sizeTextButton: 20

        onReleasedButton: {
            mainLoader.source = "GridOne.qml"
        }
    }

    Popup {
        id: popupAttention

        width: parent.width/2
        height: parent.height/2

        x: parent.width/4
        y: parent.height/4

        modal: true

        background: Rectangle {
            id: backgroundPopupAttention
            anchors.fill: parent
            color: Qt.rgba(0.8, 0.8, 0.8, 0.9)
            radius: 50
        }

        contentItem: Item {
            Text {
                anchors.centerIn: parent
                text: qsTr("Select a mode")
                font.pixelSize: 30
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    }
}


