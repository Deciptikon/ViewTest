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

            text: qsTr("Для осуществления калибровки стационарного состояния гироскопа нажмите на кнопку \"%1\" и сохраняйте состояние покоя или равномерного прямолинейного движения").arg(_buttonCalibrate.textButton)
            color: "black"
            font.pixelSize: 22

            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
        }
    }

    TextButton {
        id: _buttonCalibrate
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
            modelView.signalCalibrateZeroPointGyroscope(5000);
            backgroundText.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
            if(_progressCalibration.running) {
                return
            }
            _progressCalibration.value = 0
            _progressCalibration.start()
        }
    }

    AnimatedProgressBar {
        id: _progressCalibration

        width: 200
        height: 25

        anchors {
            top: _buttonCalibrate.bottom
            horizontalCenter: _buttonCalibrate.horizontalCenter

            topMargin: 20
        }

        from: 0
        to: 100

        value: 0

        duration: 5000
    }

    Connections {
        target: modelView

        function onSignalCalibrateZeroPointGyroscopeIsDone() {
            backgroundText.color = Qt.rgba(0.7, 1, 0.7, 0.7)
            _progressCalibration.stop()
        }
    }
}
