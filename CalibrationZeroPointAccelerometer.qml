import QtQuick 2.0
import QtQuick.Controls 2.12

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
            text: qsTr("Для осуществления калибровки стационарного состояния акселерометра нажмите на кнопку \"%1\" и сохраняйте состояние покоя или равномерного прямолинейного движения").arg(_buttonCalibrate.textButton)
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
            modelView.signalCalibrateZeroPointAccelerometer(5000);
            backgroundText.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
            _progressCalibration.value = 0
            _animation.start()
        }
    }

    ProgressBar {
        id: _progressCalibration

        height: 25
        width: 250

        anchors {
            top: _buttonCalibrate.bottom
            horizontalCenter: _buttonCalibrate.horizontalCenter

            topMargin: 20
        }

        background: Rectangle {
            //anchors.fill: _progressCalibration
            implicitWidth: 250
            implicitHeight: 25

            color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
        }

        contentItem: Item {
            implicitWidth: 250
            implicitHeight: 25

            Rectangle {
                width: _progressCalibration.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: "#17a81a"
            }
        }

        from: 0
        to: 100

        value: 0
    }

    PropertyAnimation {
        id: _animation
        target: _progressCalibration
        property: "value"
        from: _progressCalibration.from
        to: _progressCalibration.to
        duration: 5000
        running: false
        loops: 1
    }

    Connections {
        target: modelView

        function onSignalCalibrateZeroPointAccelerometerIsDone() {
            backgroundText.color = Qt.rgba(0.7, 1, 0.7, 0.7)
            _animation.stop()
        }
    }

}
