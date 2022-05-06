import QtQuick 2.0

Item {
    anchors.fill: parent

    Rectangle {
        id: testRectangleOptions

        width: parent.width/3
        height: parent.height/3
        radius: parent.width/20
        anchors.centerIn: parent
        color: Qt.rgba(Math.random(), Math.random(), Math.random(), 0.7)//Qt.rgba(0.7, 0.7, 0.7, 0.7)
        Text {
            anchors.centerIn: parent
            text: qsTr("Options Page")
            color: "black"
            font.pixelSize: 24
        }
    }


}
