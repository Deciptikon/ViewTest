import QtQuick 2.0

Item {
    id: bEdt
    property bool currentStateBool: false

    property int radiusButtonBool: 5

    property color colorActiveBool: Qt.rgba(0.2, 0.2, 0.2, 1)
    property color colorPressedBool: Qt.rgba(0.7, 0.7, 0.7, 1)
    property color colorPassiveBool: Qt.rgba(0.5, 0.5, 0.5, 0.2)
    property color colorTextButtonBool: Qt.rgba(1, 1, 1, 1)

    signal myCurrentStateBoolChanged()

//    Connections {
//        target: bEdt

//        function myCurrentStateBoolChanged() {
//            btTxtEdt.colorReleased = currentStateBool ? colorActiveBool : colorPassiveBool
//        }
//    }

    TextButton {
        id: btTxtEdt
        anchors.fill: parent

        radiusButton: radiusButtonBool

        textButton: ""

        colorReleased: currentStateBool ? colorActiveBool : colorPassiveBool
        colorPressed: colorPressedBool

        colorBorder: colorActiveBool
        widthBorderButton: 2

        onReleasedButton: {
            currentStateBool = !currentStateBool
            myCurrentStateBoolChanged()
        }
    }
}
