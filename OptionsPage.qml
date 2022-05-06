import QtQuick 2.0

Item {
    anchors.fill: parent

    Connections {
        target: TestModel

        function onSignalSendToQML(str) {
            testText.text = str
        }
    }

    Rectangle {
        id: testRectangleOptions

        width: parent.width/3
        height: parent.height/3
        radius: parent.width/20
        anchors.centerIn: parent
        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
        Text {
            id: testText
            anchors.centerIn: parent
            text: qsTr("Options Page")
            color: "black"
            font.pixelSize: 24
        }
    }

    ImageButton {
       id: btHome

       width: 64
       height: 64

       anchors.verticalCenter: parent.verticalCenter
       anchors.left: parent.left
       anchors.leftMargin: 20

       sourcePressed: "qrc:/image/bt-prev-pressed.png"
       sourceReleased: "qrc:/image/bt-prev-release.png"

       onReleasedButton: {
           console.log("released button")
           TestModel.slotTakeFromQML("<");
       }
    }

}
