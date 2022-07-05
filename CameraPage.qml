import QtQuick 2.0
import QtMultimedia 5.12

Item {
    id: _root
    anchors.fill: parent

    Item {
        id: _camera1Item
        width: parent.width/2 - 2
        height: parent.height/2 - 2

        anchors.left: parent.left
        anchors.top: parent.top

        Camera {
            id: _camera1
            objectName: "Camera1"
            deviceId: QtMultimedia.availableCameras[0].deviceId

        }

        VideoOutput {
            source: _camera1
            anchors.fill: parent
        }
    }
    Item {
        id: _camera2Item
        width: parent.width/2 - 2
        height: parent.height/2 - 2

        anchors.right: parent.right
        anchors.top: parent.top

        Camera {
            id: _camera2
            objectName: "Camera2"
            deviceId: QtMultimedia.availableCameras[1].deviceId

        }

        VideoOutput {
            source: _camera2
            anchors.fill: parent
        }
    }
}
