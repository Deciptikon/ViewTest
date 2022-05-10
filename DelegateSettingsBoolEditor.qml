import QtQuick 2.0

Item {
    anchors.fill: parent

    Text {
        text: title
        font.pixelSize: 16
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    BoolValueEditior {
        id: bEdit
        width: height
        height: parent.height - 5

        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        currentStateBool: value

        onMyCurrentStateBoolChanged: {
            value = currentStateBool
            console.log("value =" + value)
        }
    }
}
