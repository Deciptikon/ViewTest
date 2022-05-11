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
            text: qsTr("QuickSettingsTwo.qml")
            color: "black"
            font.pixelSize: 40
        }
    }

    TextButton {
        id: btNextQiuckSettingsOne
        width: 60
        height: width

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 15
        }

        textButton: qsTr("Next")
        sizeTextButton: 20

        onReleasedButton: {
            stackViewQuickSettings.push("QuickSettingsThree.qml")
        }
    }
    TextButton {
        width: 60
        height: width

        anchors {
            right: btNextQiuckSettingsOne.left
            bottom: parent.bottom
            margins: 15
        }

        textButton: qsTr("Back")
        sizeTextButton: 20

        onReleasedButton: {
            stackViewQuickSettings.pop()
        }
    }
}
