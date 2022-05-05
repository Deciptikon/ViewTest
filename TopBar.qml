import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: topBar

    property real sizeIcons//: 32
    property real marginIcons//: 50

    anchors.top:  parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    height: sizeIcons + 2*marginIcons

    Rectangle {
        id: rectTopBar
        anchors.fill: parent
        color: Qt.rgba(0.5, 0.5, 0.5, 0.5)//"#88888888"

        ImageButton {
           id: btHome

           width: sizeIcons
           height: sizeIcons

           anchors.verticalCenter: parent.verticalCenter
           anchors.left: parent.left
           anchors.leftMargin: 2*marginIcons

           sourcePressed: "qrc:/image/bt-home-pressed.png"
           sourceReleased: "qrc:/image/bt-home-release.png"

           onReleasedButton: {
               console.log("released button Home!!!")
           }
        }
    }


}
