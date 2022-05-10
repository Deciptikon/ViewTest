import QtQuick 2.0

Item {
    anchors.fill: parent
    Text {
        text: title
        font.pixelSize: 16
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        width: 150
        height: parent.height - 5
        anchors.right: btSetValue.left
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        radius: 5

        color: Qt.rgba(0.7, 0.7, 0.7, 1)

        border.color: Qt.rgba(0.2, 0.2, 0.2, 1)
        border.width: 2

        clip: true

        TextInput {
            id: txtInput
            anchors.fill: parent
            anchors.topMargin: 2
            anchors.rightMargin: 2
            anchors.leftMargin: 2

            text: value
            font.pixelSize: 24

            Keys.onEnterPressed: {
                btSetValue.releasedButton()
                //value = txtInput.text
            }
        }
    }
    TextButton {
        id: btSetValue

        width: height
        height: parent.height - 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        radiusButton: 5

        widthBorderButton: 2
        colorBorder: Qt.rgba(0.2, 0.2, 0.2, 1)

        textButton: "set"
        sizeTextButton: 16
        onReleasedButton: {
            value = txtInput.text
        }
    }
}
