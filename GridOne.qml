import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {

    ListModel {
        id: listModel

        ListElement {name: "One";   page: ""; imageSource: ""}
        ListElement {name: "Two";   page: ""; imageSource: ""}
        ListElement {name: "Three"; page: ""; imageSource: ""}
        ListElement {name: "Four";  page: ""; imageSource: ""}
        ListElement {name: "Five";  page: ""; imageSource: ""}
        ListElement {name: "Six";   page: ""; imageSource: ""}
        ListElement {name: "Seven"; page: ""; imageSource: ""}
        ListElement {name: "Options"; page: "OptionsPage.qml"; imageSource: "qrc:/image/icon-settings.png"}
    }

    GridView {
        id: gridView

        anchors.fill: parent
        anchors.leftMargin: 40

        model: listModel

        cellWidth: 240
        cellHeight: 240

        delegate: Rectangle {
            width: 200
            height: 200

            //color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
            color: Qt.rgba(0.5, 0.5, 0.5, 0.5)

            radius: 15

            Image {
                anchors.fill: parent
                source: imageSource
            }

            Text {
                color: "black"
                text: name
                font.pixelSize: 18
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mainLoader.source = page
                }
            }
        }
    }
}
