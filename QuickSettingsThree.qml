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
            text: qsTr("QuickSettingsPageThree.qml")
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
            // здесь либо настройки заканчиваются и переходим на страницу автовождения
            mainLoader.source = "DrivePage.qml"

            // либо на следующую страницу настроек
            // а с неё на следующую, либо на страницу автовождения
            // stackViewQuickSettings.push("QuickSettingsFour.qml")

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
