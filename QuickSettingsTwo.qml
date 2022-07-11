import QtQuick 2.0

Item {

    Item {
        id: _itemWidthBetweenLines
        height: 80

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right

            margins: 5
        }

        Rectangle {
            anchors.fill: parent
            radius: 10

            Text {
                id: _textItemWidthBetweenLines

                width: parent.width/2
                height: parent.height

                anchors {
                    top: parent.top
                    left: parent.left

                    leftMargin: 20
                }

                    text: qsTr("Width Between Lines, in meters")

                    color: "black"
                    font.pixelSize: 22

                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                }

                TextButton {
                    id: _setWidthBetweenLines

                    width: parent.height
                    height: parent.height

                    anchors {
                        right: parent.right
                        top: parent.top

                        rightMargin: 10
                    }

                    textButton: qsTr("Set")
                    sizeTextButton: 22

                    onReleasedButton: {
                        console.log(_editWidthBetweenLines.currentValueDouble)
                        modelView.setWidthBetweenLines(_editWidthBetweenLines.currentValueDouble)
                        modelView.set
                    }
                }

                DoubleValueEditor {
                    id: _editWidthBetweenLines

                    width: parent.height*3
                    height: parent.height

                    anchors {
                        right: _setWidthBetweenLines.left
                        top: parent.top

                        rightMargin: 10
                    }

                    minValueDouble: 1
                    maxValueDouble: 99

                    currentValueDouble: modelView.getWidthBetweenLines()
                }
        }
    }

    TextButton {
        id: btNextQiuckSettingsOne
        width: 80
        height: width

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 15
        }

        textButton: qsTr("Next")
        sizeTextButton: 20

        onReleasedButton: {
            stackViewQuickSettings.push("QuickSettingsThree.qml")
        }
    }
    TextButton {
        width: 80
        height: width

        anchors {
            right: btNextQiuckSettingsOne.left
            bottom: parent.bottom
            margins: 15
        }

        textButton: qsTr("Back")
        sizeTextButton: 20

        onReleasedButton: {
            stackViewQuickSettings.pop()
        }
    }
}
