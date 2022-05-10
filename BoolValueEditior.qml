import QtQuick 2.0

Item {
    id: bEdt
    property bool currentStateBool: value

    property real radiusButtonBool: 5

    property color colorActiveBool: Qt.rgba(0.2, 0.2, 0.2, 1)
    property color colorPressedBool: Qt.rgba(0.7, 0.7, 0.7, 1)
    property color colorPassiveBool: Qt.rgba(0.5, 0.5, 0.5, 0.2)
    property color colorTextButtonBool: Qt.rgba(1, 1, 1, 1)

    signal myCurrentStateBoolChanged()

    Rectangle {
        id: rectBtBool

//        width: height
//        height: parent.height - 5

//        anchors.right: parent.right
//        anchors.rightMargin: 10
//        anchors.verticalCenter: parent.verticalCenter
        anchors.fill: parent

        radius: radiusButtonBool

        color: currentStateBool ? colorActiveBool : colorPassiveBool

        border.color: colorActiveBool
        border.width: 2

        MouseArea {
            anchors.fill: parent

            onPressed: {
                rectBtBool.color = Qt.rgba(0.7, 0.7, 0.7, 1)
            }

            onReleased: {


//                if(currentStateBool) {
//                    currentStateBool = false
//                    console.log("value:  true --> false")
//                } else {
//                    currentStateBool = true
//                    console.log("value:  false --> true")
//                }
                currentStateBool = !currentStateBool
                console.log("currentStateBool =" + currentStateBool)
                myCurrentStateBoolChanged()

                rectBtBool.color = currentStateBool ? colorActiveBool : colorPassiveBool

            }
            onCanceled: {
                rectBtBool.color = currentStateBool ? colorActiveBool : colorPassiveBool
            }
        }
    }
}
