import QtQuick 2.0

Item {
    id: check

    property var sourceTrue
    property var sourceFalse

    property bool value: false

    signal changedImageCheckBox()

    Image {
        id: imgCheck
        anchors.fill: parent
        source: value == true ? sourceTrue : sourceFalse
    }

    MouseArea {
        id: mouseAreaCheck
        anchors.fill: parent

        onReleased: {
            value = !value
            imgCheck.source = value == true ? sourceTrue : sourceFalse
            changedImageCheckBox()
        }
    }
}
