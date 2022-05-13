import QtQuick 2.0

Item {

    Rectangle {
        width: parent.width/2
        height: parent.height/2
        radius: parent.width/20
        anchors.centerIn: parent
        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
        Text {
            id: testText
            anchors.centerIn: parent
            text: qsTr("QuickSettingsOne.qml")
            color: "black"
            font.pixelSize: 40
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
}
