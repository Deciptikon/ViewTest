import QtQuick 2.0

Item {
    anchors.fill: parent

    Rectangle {
        id: backgroundText
        width: parent.width - 20
        height: parent.height/3
        radius: parent.width/20
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 10
        }

        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
        Text {
            id: testText
            anchors.centerIn: parent
            anchors.fill: parent
            anchors.margins: 20

            text: qsTr("Для осуществления калибровки нажмите на кнопку \"Calibrate\" и сохраняйте состояние покоя или равномерного прямолинейного движения")
            color: "black"
            font.pixelSize: 22

            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
        }
    }

    TextButton {
        height: parent.height/5
        width: parent.width/4

        sizeTextButton: 20
        textButton: qsTr("Calibrate")

        anchors {
            top: backgroundText.bottom
            horizontalCenter: backgroundText.horizontalCenter

            topMargin: 50
        }

        onReleasedButton: {
            modelView.signalCalibrateZeroPointAccelerometer(5000);
            backgroundText.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
        }
    }

    Connections {
        target: modelView

        function onSignalCalibrateZeroPointAccelerometerIsDone() {
            backgroundText.color = Qt.rgba(0.7, 1, 0.7, 0.7)
        }
    }

}
