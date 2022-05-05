import QtQuick 2.12
import QtQuick.Window 2.12

Window {

    property real sizeInter: 32
    property real marginInter: 5

    width: 800
    height: 480
    visible: true
    title: qsTr("Hello World")

    Image {
        id: background
        source: "qrc:/image/nature-grass.jpg"
        anchors.fill: parent
    }

    TopBar {
        id: statusBar
        sizeIcons: sizeInter
        marginIcons: marginInter
    }
}
