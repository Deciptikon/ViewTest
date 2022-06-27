import QtQuick 2.0

Item {
    anchors.fill: parent

    ListModel {
        id: listCalibration
        ListElement {
            nameMode: "Calibration of the zero position of the accelerometer";
            pageMode: "CalibrationZeroPointAccelerometer.qml";

        }
        ListElement {
            nameMode: "Calibration of the zero position of the gyroscope";
            pageMode: "CalibrationZeroPointGyroscope.qml";
        }

        ListElement {
            nameMode: "Calibration of the Z-axis of the gyroscope";
            pageMode: "CalibrationZAxisGyroscope.qml";
        }
    }

    ListView {
        id: viewListCalibration
        anchors.fill: parent
        anchors.margins: 15

        model: listCalibration
        spacing: 3

        delegate: Rectangle {
            width: viewListCalibration.width
            height: 100
            radius: 10
            color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
            Text {
                text: nameMode

                anchors.fill: parent
                anchors.margins: 10

                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                //horizontalAlignment: Text.AlignHCenter

                font.pixelSize: 22
            }

            MouseArea {
                anchors.fill: parent

                onPressed: {
                    parent.color = Qt.rgba(0.7, 0.7, 0.7, 0.9)
                }
                onReleased: {
                    mainLoader.source = pageMode
                }
                onCanceled: {
                    parent.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
                }
            }

        }
    }
}
