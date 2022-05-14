import QtQuick 2.0

Item {
    //
    property int lastMode: 0
    property int currentMode: 0

    ListModel {
        id: listModeDrive

        ListElement {
            name: qsTr("Key points");
            mode: "keyPoints";
            imageSource: "qrc:/image/icon-way-keypoins.png"
            //isActive: true
        }
        ListElement {
            name: qsTr("Snake");
            mode: "snake";
            imageSource: "qrc:/image/icon-way-snake.png"
            //isActive: false
        }
        ListElement {
            name: qsTr("Spiral");
            mode: "spiral";
            imageSource: "qrc:/image/icon-way-spiral.png"
            //isActive: false
        }
        ListElement {
            name: qsTr("None");
            mode: "none";
            imageSource: "qrc:/image/icon-no-image.png"
            //isActive: false
        }
        ListElement {
            name: qsTr("None");
            mode: "none";
            imageSource: "qrc:/image/icon-no-image.png"
            //isActive: false
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

    GridView {
        id: gridViewMode

        anchors.fill: parent
        anchors.leftMargin: 60
        anchors.topMargin: 60

        model: listModeDrive

        cellWidth: 240
        cellHeight: 220

        clip: true // обрезка за пределами окна
        focus: true

        delegate: Rectangle {
            property bool isActive: false

            id: delegateGridViewMode

            width: 200
            height: 200
            radius: 15

            color: isActive ? Qt.rgba(0.4, 0.4, 0.4, 0.9) : Qt.rgba(0.5, 0.5, 0.5, 0.7)

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
                }
                onReleased: {
                    currentMode = index
                    console.log(mode)

                    delegateGridViewMode.isActive = true
                    lastMode = currentMode

                    gridViewMode.update()
                    //delegateGridViewMode.color = Qt.rgba(0.5, 0.5, 0.5, 0.7)
                    //model.setDriveMode(mode)
                }
                onCanceled: {
                    delegateGridViewMode.color = Qt.rgba(0.5, 0.5, 0.5, 0.7)
                }
            }
        }
    }
}


