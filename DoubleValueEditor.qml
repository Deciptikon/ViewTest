import QtQuick 2.0

Item {
    id: dbValEditor

    width: 120
    height: 40

    property real maxValueDouble: 100
    property real minValueDouble: 0

    property real currentValueDouble:  50
    property real stepEditValueDouble: 1

    property real sizeTextButtonDouble: 24
    property real sizeTextViewDouble:   24

    property real radiusDoubleEditor: 3

    property color colorPressedDouble:    Qt.rgba(0.5, 0.5, 0.5, 1)
    property color colorReleasedDouble:   Qt.rgba(0.7, 0.7, 0.7, 1)
    property color colorTextButtonDouble: Qt.rgba(0, 0, 0, 1)
    property color colorTextViewDouble:   Qt.rgba(0, 0, 0, 1)

    signal myCurrentValueDoubleChanged()

    TextButton {
        id: btOut
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: parent.height
        height: parent.height
        radiusButton: radiusDoubleEditor

        widthBorderButton: 2
        colorBorder: Qt.rgba(0.2, 0.2, 0.2, 1)

        textButton: "-"
        sizeTextButton: sizeTextButtonDouble
        onReleasedButton: {
            var newVal = currentValueDouble - stepEditValueDouble

            if(newVal < minValueDouble) {
                newVal = minValueDouble
            }
            if(newVal > maxValueDouble) {
                newVal = maxValueDouble
            }

            if(newVal == currentValueDouble) {
                return
            }

            currentValueDouble = newVal
            myCurrentValueDoubleChanged()
        }
    }

    Rectangle {
        height: btOut.height
        anchors {
            left: btOut.right
            right: btIn.left
            leftMargin: 5
            rightMargin: 5
            verticalCenter: btOut.verticalCenter
        }
        radius: 5
        color: colorReleasedDouble

        border.width: 2
        border.color: Qt.rgba(0.2, 0.2, 0.2, 1)

        clip: true

        Text {
            id: viewText

            anchors.centerIn: parent

            text: currentValueDouble
            font.pixelSize: sizeTextViewDouble
            color: colorTextViewDouble
        }
    }

    TextButton {
        id: btIn
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: parent.height
        height: parent.height
        radiusButton: radiusDoubleEditor

        widthBorderButton: 2
        colorBorder: Qt.rgba(0.2, 0.2, 0.2, 1)

        textButton: "+"
        sizeTextButton: sizeTextButtonDouble
        onReleasedButton: {
            var newVal = currentValueDouble + stepEditValueDouble

            if(newVal < minValueDouble) {
                newVal = minValueDouble
            }
            if(newVal > maxValueDouble) {
                newVal = maxValueDouble
            }

            if(newVal == currentValueDouble) {
                return
            }

            currentValueDouble = newVal
            myCurrentValueDoubleChanged()
        }
    }
}
