import QtQuick 2.12
import QtQuick.Window 2.12



Window {

    property real sizeInter: 32
    property real marginInter: 5

    width: 800
    height: 480
    visible: true
    title: qsTr("AgroSlave")

    Image {
        id: background
        source: "qrc:/image/nature-grass.jpg"
        anchors.fill: parent
    }

    Loader {
        id: mainLoader

        anchors {
            top: statusBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        source: "StackViewPage.qml"
    }

    TopBar {
        id: statusBar
        sizeIcons: sizeInter
        marginIcons: marginInter

        onActionBtHome: {
            model.testMetod("ActionBtHome")
            mainLoader.source = "StackViewPage.qml"
        }

    }
}
