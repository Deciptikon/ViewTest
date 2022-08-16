import QtQuick 2.0

Item {
    id: root
    anchors.fill: parent

    property double angleWheel: 0

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

            text: qsTr("Для осуществления калибровки датчика угла поворота нажмите на кнопку \"%1\" и вращайте руль до упора в одну сторону, а затем в другую, затем нажмите кнопку \"%1\" снова.\nВ случае успешного прохождения калибровки это сообщение окрасится в зеленый цвет.").arg(_buttonCalibrate.textButton)
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
        textButton: qsTr("Calibrate Angle Wheel")

        anchors {
            top: backgroundText.bottom
            horizontalCenter: backgroundText.horizontalCenter

            topMargin: 50
        }

        onReleasedButton: {
            backgroundText.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
            modelView.signalCalibrateWheel();
        }
    }
    Rectangle {
        id: _rectWheelData

        height: parent.height/5
        width: parent.width/4

        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

        anchors {
            top: _buttonCalibrate.bottom
            horizontalCenter: _buttonCalibrate.horizontalCenter

            topMargin: 50
        }
        Text {
            id: _dataWheelAngle
            anchors.fill: parent
            text: qsTr("Angle: %1").arg(angleWheel)
            color: "black"
            font.pixelSize: 22
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

    }

    Connections {
        target: modelView

        function onSignalDataWheelToQML(ang) {
            angleWheel = ang

        }

        function onSignalCalibrateWheelIsDone() {
            backgroundText.color = Qt.rgba(0.7, 1, 0.7, 0.7)
        }
    }
}
