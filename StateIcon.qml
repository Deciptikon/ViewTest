import QtQuick 2.0

Item {
    property bool stateIsActive: false

    property real widthBorderState: 2
    property color colorBorderState: Qt.rgba(0.2, 0.2, 0.2, 1)

    property string sourceImagePassive
    property string sourceImageActive

    Rectangle {
        anchors.fill: parent
        radius: 5

        border.width: widthBorderState
        border.color: colorBorderState

        color: Qt.rgba(1, 1, 1, 0)

        Image {
            anchors.fill: parent
            anchors.margins: 1
            source: stateIsActive ? sourceImageActive : sourceImagePassive
        }
    }
}
