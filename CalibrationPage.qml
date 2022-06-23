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
            nameMode: "Calibrationof the zero positionof the zero position of the zero position of the gyroscope";
            pageMode: "";
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
            color: "lightgray"
            Text {
                text: nameMode

                anchors.fill: parent
                anchors.margins: 5

                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                //horizontalAlignment: Text.AlignHCenter

                font.pixelSize: 22
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    mainLoader.source = pageMode
                }
            }

        }
    }
}
