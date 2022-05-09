import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
    anchors.fill: parent

    ListModel {
        id: listSettings

        ListElement{
            name: "Option1"
            size: "132.00"
        }
        ListElement{
            name: "Option2"
            size: "51"//appSettings.getValueString("Option2")
        }
        ListElement{
            name: "Option3"
            size: "95.23"
        }
        ListElement{
            name: "Option4"
            size: "8462.5"
        }
        ListElement{
            name: "Option5"
            size: "1578.65"
        }
    }

    ListView {
        id: viewListSettings
        anchors.fill: parent

        model: appSettings//listSettings
        delegate: Item {
            id: itemDelegateSettings

            width: viewListSettings.width
            height: 50

            Rectangle {
                id: rectDelegateSettings

                anchors.fill: parent
                anchors {
                    topMargin: 5
                    leftMargin: 5
                    rightMargin: 5
                }


                radius: 10

                color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

                Column {
                    anchors {
                        left: parent.left
                        right: itemInputData.left
                        top: parent.top
                        bottom: parent.bottom

                        leftMargin: 5
                    }

                    Text {
                        text: title
                        font.pixelSize: 16
                    }
                    Text {
                        text: value
                        font.pixelSize: 16
                    }
                }
                Item {
                    id: itemInputData
                    width: rectDelegateSettings.width/2
                    height: rectDelegateSettings.height
                    anchors.right: rectDelegateSettings.right
                    anchors.verticalCenter: rectDelegateSettings.verticalCenter

                    TextButton {
                        id: btTest
                        width: 100
                        height: 30
                        radiusButton: 5
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        textButton: "button" + index
                        onReleasedButton: {
                            console.log("press button")
                            //name = "--------"
                            //appSettings.setValue("Option1", "99999999")
                            //size = appSettings.getValue("Option1")
                            //console.log(size)
                            value = "228" + index

                        }
                    }
                }
            }
        }
    }
}
