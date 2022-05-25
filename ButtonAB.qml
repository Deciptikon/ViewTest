import QtQuick 2.0

Item {
    id: buttonAB

    property var sourceA
    property var sourceB

    property int value: 1

    signal changedButtonAB()

    Image {
        id: imgAB
        anchors.fill: parent
        source: value == 1 ? sourceA : sourceB
    }

    MouseArea {
        id: mouseAreaAB
        anchors.fill: parent

        onReleased: {
            value++
            imgAB.source = value == 1 ? sourceA : sourceB
            changedButtonAB()
            if(value > 2) {
                buttonAB.visible = false
                buttonAB.enabled = false
            }
        }
    }
}
