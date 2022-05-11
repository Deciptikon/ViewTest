import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {

    ListModel {
        id: listModel

        ListElement {
            name: qsTr("Quick Settings");
            page: "QuickSettingsPage.qml";
            imageSource: "qrc:/image/icon-quick-settings.png"
        }
        ListElement {
            name: qsTr("Drive");
            page: "DrivePage.qml";
            imageSource: "qrc:/image/icon-tractor.png"
        }
        ListElement {
            name: qsTr("Camera view");
            page: "CameraPage.qml";
            imageSource: "qrc:/image/icon-cam.png"
        }
        ListElement {
            name: qsTr("My Fields");
            page: "FieldsPage.qml";
            imageSource: "qrc:/image/icon-field-base.png"
        }
        ListElement {
            name: qsTr("Options");
            page: "OptionsPage.qml";
            imageSource: "qrc:/image/icon-settings.png"
        }
        ListElement {
            name: qsTr("Info");
            page: "InfoPage.qml";
            imageSource: "qrc:/image/icon-info.png"
        }
        ListElement {
            name: qsTr("Seven");
            page: "";
            imageSource: "qrc:/image/icon-no-image.png"
        }
        ListElement {
            name: qsTr("--8--");
            page: "";
            imageSource: "qrc:/image/icon-no-image.png"
        }
        // здесь можно добавить или удалить кнопки для страниц
    }

    GridView {
        id: gridView

        anchors.fill: parent
        anchors.leftMargin: 60
        anchors.topMargin: 10

        model: listModel

        cellWidth: 240
        cellHeight: 220

        //clip: true // обрезка за пределами окна

        delegate: Rectangle {
            id: delegatGridView
            width: 200
            height: 200
            radius: 15

            color: Qt.rgba(0.5, 0.5, 0.5, 0.7)

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

                onPressed: {
                    delegatGridView.color = Qt.rgba(0.4, 0.4, 0.4, 0.9)
                }
                onReleased: {
                    delegatGridView.color = Qt.rgba(0.5, 0.5, 0.5, 0.7)
                    mainLoader.source = page
                }
                onCanceled: {
                    delegatGridView.color = Qt.rgba(0.5, 0.5, 0.5, 0.7)
                }
            }
        }

        focus: true
    }
}
