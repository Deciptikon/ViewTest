import QtQuick 2.0


Item {
    id: bt

    width: 60
    height: 40

    property string textButton: "Button"
    property int sizeTextButton: 16
    property int radiusButton: 3

    property color colorPressed: Qt.rgba(0.5, 0.5, 0.5, 1)
    property color colorReleased: Qt.rgba(0.7, 0.7, 0.7, 1)
    property color colorTextButton: Qt.rgba(0, 0, 0, 1)

    signal releasedButton()

    Rectangle {
        id: rectBt

        anchors.fill: parent
        radius: radiusButton
        color: colorReleased

        Text {
            id: textBt
            anchors.centerIn: parent
            text: textButton
            font.pixelSize: sizeTextButton
            color: colorTextButton
        }

        MouseArea {
            anchors.fill: parent

            onPressed: {
                rectBt.color = colorPressed
            }

            onReleased: {
                rectBt.color = colorReleased
                releasedButton()
            }
            onCanceled: {
                rectBt.color = colorReleased
            }
        }
    }



}
