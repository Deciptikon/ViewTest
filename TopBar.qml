import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: topBar

    property real sizeIcons
    property real marginIcons

    signal actionBtHome()

    anchors.top:  parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    height: sizeIcons + 2*marginIcons

    Rectangle {
        id: rectTopBar
        anchors.fill: parent
        color: Qt.rgba(0.5, 0.5, 0.5, 0.5)

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
               actionBtHome()
           }
        }

        // отражаем изменения состояний по сигналу из модели
        Connections {
            target: model

            function onSignalStateGPStoQML(state) {// bool state
                console.log("function onSignalStateGPStoQML(state)")
                stateGPS.stateIsActive = state
            }
            function onSignalStateI2CtoQML(state) {// bool state
                console.log("function onSignalStateI2CtoQML(state)")
                stateI2CWheel.stateIsActive = state
            }
        }

        StateIcon {
            id: stateGPS

            width: sizeIcons
            height: sizeIcons
            anchors {
                right: parent.right
                rightMargin: 15
                verticalCenter: parent.verticalCenter
            }
            widthBorderState: 1

            sourceImageActive:  "qrc:/image/state-gps-active.png"
            sourceImagePassive: "qrc:/image/state-gps-passive.png"
        }
        StateIcon {
            id: stateI2CWheel

            width: sizeIcons
            height: sizeIcons
            anchors {
                right: stateGPS.left
                rightMargin: 15
                verticalCenter: parent.verticalCenter
            }
            widthBorderState: 1

            sourceImageActive:  "qrc:/image/state-wheel-active.png"
            sourceImagePassive: "qrc:/image/state-wheel-passive.png"
        }
    }
}
