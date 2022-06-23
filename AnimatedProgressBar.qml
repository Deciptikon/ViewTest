import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root
    property color backgroundColor: Qt.rgba(0.7, 0.7, 0.7, 0.7)
    property color contentColor: "#17a81a"
    property int from: 0
    property int to: 100
    property int value: 0
    property int duration: 5000
    property bool running: false

    function start() {
        root.running = true
    }
    function stop() {
        root.running = false
    }

    ProgressBar {
        id: _progress

        anchors.fill: root

        background: Rectangle {
            implicitWidth: _progress.width
            implicitHeight: _progress.height

            color: backgroundColor
        }

        contentItem: Item {
            implicitWidth: _progress.width
            implicitHeight: _progress.height

            Rectangle {
                width: _progress.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: contentColor
            }
        }

        from: root.from
        to: root.to

        value: root.value
    }

    PropertyAnimation {
        id: _animation
        target: _progress
        property: "value"
        from: root.from
        to: root.to
        duration: root.duration
        running: root.running
        loops: 1
    }
}
