import QtQuick 2.0
import QtQuick.Controls 2.12

//Item {
    Popup {
        id: popupAttention

        property string textAttention: qsTr("Attention")
        property color colorTextAttention: Qt.rgba(0, 0, 0, 1)
        property color colorBackgroundAttention: Qt.rgba(0.8, 0.8, 0.8, 0.9)

        width: parent.width/2
        height: parent.height/2

        x: parent.width/4
        y: parent.height/4

        modal: true

        background: Rectangle {
            id: backgroundPopupAttention
            anchors.fill: parent
            color: colorBackgroundAttention
            radius: 50
        }

        contentItem: Item {
            Text {
                anchors.centerIn: parent
                text: textAttention
                font.pixelSize: 30
                color: colorTextAttention
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    }
//}
