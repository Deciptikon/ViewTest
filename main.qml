import QtQuick 2.12
import QtQuick.Window 2.12


Window {
    id: mainwindow

    width: 800
    height: 600
    visible: true
    title: qsTr("AgroSlave")

    // если Linux - во весь экран
    visibility: modelView.isLinux()? Window.FullScreen : Window.AutomaticVisibility
    flags: if(!modelView.isLinux()) Qt.FramelessWindowHint
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

        onSourceChanged: {
            if(source == "qrc:/GridOne.qml") {
                statusBar.enablePowerOff = true
                return
            }
            statusBar.enablePowerOff = false

            console.log(source)
            console.log(" ")
            console.log(" ")
        }
    }

    TopBar {
        id: statusBar
        sizeIcons: 50
        marginIcons: 5


        onActionBtHome: {
            mainLoader.source = "GridOne.qml"
        }

        onActionPowerOff: {
            mainwindow.close()
            // пока здесь просто закрытие окна, но в будущем можно
            // сделать выключение питания Raspberry
        }

    }
}
