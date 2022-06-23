import QtQuick 2.0

Item {
    anchors.fill: parent

    Rectangle {
        width: parent.width/2
        height: parent.height/2
        radius: parent.width/20
        anchors.centerIn: parent
        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
        Text {
            id: testText
            anchors.centerIn: parent
            text: qsTr("Calibration Zero Point Gyroscope")
            color: "black"
            font.pixelSize: 40
        }
    }
}
