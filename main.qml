import QtQuick 2.12
import QtQuick.Window 2.12


Window {

    width: 800
    height: 600
    visible: true
    title: qsTr("AgroSlave")

    //visibility: Window.FullScreen

//    onClosing: {
//        appSettings.checkAndRestoreSettings()
//    }

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
        source: "GridOne.qml"
    }

    TopBar {
        id: statusBar
        sizeIcons: 50
        marginIcons: 5


        onActionBtHome: {
            modelView.testMetod("ActionBtHome")
            mainLoader.source = "GridOne.qml"
        }

    }
}
