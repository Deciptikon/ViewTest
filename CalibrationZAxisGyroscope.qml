import QtQuick 2.0

Item {
    id: root
    anchors.fill: parent

    property double gyroX: 0
    property double gyroY: 0
    property double gyroZ: 0

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

            text: qsTr("Для осуществления калибровки оси Z гироскопа начните поворот и нажмите на кнопку \"%1\", сохраняйте состояние поворота до исходного положения, потом нажмите кнопку \"%1\" снова.\nВ случае успешного прохождения калибровки это сообщение окрасится в зеленый цвет.").arg(_buttonCalibrate.textButton)
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
        textButton: qsTr("Calibrate Z Axis")

        anchors {
            top: backgroundText.bottom
            horizontalCenter: backgroundText.horizontalCenter

            topMargin: 50
        }

        onReleasedButton: {
            backgroundText.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
            modelView.signalCalibrateZAxisGyroscope();
        }
    }
    Rectangle {
        id: _rectGyroData

        height: parent.height/5
        width: parent.width/4

        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

        anchors {
            top: _buttonCalibrate.bottom
            horizontalCenter: _buttonCalibrate.horizontalCenter

            topMargin: 50
        }

        Column {
            anchors.fill: parent

            Text {
                id: _dataGyroX
                text: qsTr("X: %1").arg(gyroX)
                color: "black"
                font.pixelSize: 22
            }
            Text {
                id: _dataGyroY
                text: qsTr("Y: %1").arg(gyroY)
                color: "black"
                font.pixelSize: 22
            }
            Text {
                id: _dataGyroZ
                text: qsTr("Z: %1").arg(gyroZ)
                color: "black"
                font.pixelSize: 22
            }
        }
    }

    Connections {
        target: modelView

        function onSignalDataGyrosToQML(x, y, z) {
            gyroX = x
            gyroY = y
            gyroZ = z
        }

        function onSignalCalibrateZAxisGyroscopeIsDone() {
            console.log("0000000000000000000000000000000000000000000000000000000000000000000000000")
            backgroundText.color = Qt.rgba(0.7, 1, 0.7, 0.7)
        }
    }

}
