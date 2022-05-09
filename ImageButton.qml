import QtQuick 2.0

Item {
    id: bt

    property var sourcePressed
    property var sourceReleased

    signal releasedButton()

    Image {
        id: imgBt

        anchors.fill: parent

        source: sourceReleased
    }

    MouseArea {
        id: mouseAreaBt

        anchors.fill: parent

        onPressed: {
            imgBt.source = sourcePressed
        }

        onReleased: {
            imgBt.source = sourceReleased
            releasedButton()
        }
    }
}
