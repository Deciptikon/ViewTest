import QtQuick 2.0

Item {
    property bool stateGPSActive: false

    property real widthBorderStateGPS: 2
    property color colorBorderStateGPS: Qt.rgba(0.2, 0.2, 0.2, 1)


    Rectangle {
        anchors.fill: parent
        radius: 5

        border.width: widthBorderStateGPS
        border.color: colorBorderStateGPS

        color: Qt.rgba(1, 1, 1, 0)

        Image {
            anchors.fill: parent
            anchors.margins: 1
            source: stateGPSActive ? "qrc:/image/state-gps-active.png" : "qrc:/image/state-gps-passive.png"
        }
    }
}
