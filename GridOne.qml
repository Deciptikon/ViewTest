import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {

    ListModel {
        id: listModel

        ListElement {name: "One"; page: ""}
        ListElement {name: "Two"; page: ""}
        ListElement {name: "Three"; page: ""}
        ListElement {name: "Four"; page: ""}
        ListElement {name: "Five"; page: ""}
        ListElement {name: "Six"; page: ""}
        ListElement {name: "Seven"; page: ""}
        ListElement {name: "Options"; page: "OptionsPage.qml"}
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

            color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)

            radius: 5

            Text {
                color: "black"
                text: name
                anchors.centerIn: parent
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
