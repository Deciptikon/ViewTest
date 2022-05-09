import QtQuick 2.0

Item {

    property real maxValue: 100
    property real minValue: 0

    property real currentValue: 50
    property real stepEdit: 1

    property real sizeTextButton: 24
    property real sizeTextView: 24

    property color colorPressed: Qt.rgba(0.5, 0.5, 0.5, 1)
    property color colorReleased: Qt.rgba(0.7, 0.7, 0.7, 1)
    property color colorTextButton: Qt.rgba(0, 0, 0, 1)
    property color colorTextView: Qt.rgba(0, 0, 0, 1)

    signal curValueChanged()

//    Connections {
//        target: btOut

//        function onCurrentValueChanged() {
//            viewText.text = currentValue
//        }
//    }


    width: 120
    height: 40

    TextButton {
        id: btOut
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        height: parent.height

        textButton: "-"
        sizeTextButton: sizeTextButton
        onReleasedButton: {
            var newVal = currentValue - stepEdit

            if(newVal < minValue) {
                newVal = minValue
            }
            if(newVal > maxValue) {
                newVal = maxValue
            }

            if(newVal == currentValue) {
                return
            }

            currentValue = newVal
            curValueChanged()
        }
    }

    Text {
        id: viewText

        anchors {
            left: btOut.right
            right: btIn.left
            top: parent.top
            bottom: parent.bottom
        }

        text: currentValue
        font.pixelSize: sizeTextView
        color: colorTextView
    }

    TextButton {
        id: btIn
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.height
        height: parent.height

        textButton: "+"
        sizeTextButton: sizeTextButton
        onReleasedButton: {
            var newVal = currentValue + stepEdit

            if(newVal < minValue) {
                newVal = minValue
            }
            if(newVal > maxValue) {
                newVal = maxValue
            }

            if(newVal == currentValue) {
                return
            }

            currentValue = newVal
            curValueChanged()
        }
    }
}
