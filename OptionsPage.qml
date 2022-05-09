import QtQuick 2.0
import QtQuick.Layouts 1.12

import TypeEdit 1.0 // способ редактирования настроек

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

            property bool isDoubleValueEdit: true

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
//                    Text {
//                        text: value
//                        font.pixelSize: 16
//                    }

                }
                Item {
                    id: itemInputData
                    width: rectDelegateSettings.width/2
                    height: rectDelegateSettings.height
                    anchors.right: rectDelegateSettings.right
                    anchors.verticalCenter: rectDelegateSettings.verticalCenter

                    DoubleValueEditor {
                        id: dbEdit
                        width: 120
                        height: 36
                        radiusDoubleEditor: 5

                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter

                        maxValueDouble: 1000
                        stepEditValueDouble: 10
                        currentValueDouble: value

                        visible: isDoubleValueEdit

                        onMyCurrentValueDoubleChanged: {
                            value = currentValueDouble
                        }
                        // здесь можно добавить кнопку установки значения,
                        // и перенести в неё код из onMyCurrentValueDoubleChanged
                        // если не нужно сохранять настройки при каждом изменении
                    }

//                    TextButton {
//                        id: btTest
//                        width: 100
//                        height: 30
//                        radiusButton: 5
//                        anchors.right: parent.right
//                        anchors.rightMargin: 5
//                        anchors.verticalCenter: parent.verticalCenter
//                        textButton: "rndEdit" + index
//                        onReleasedButton: {
//                            console.log("press button")

//                            value = Math.ceil(Math.random()*1000)
//                            console.log(value)

//                            if(typeEdit == TypeEdit.BOOL_EDIT) {
//                                console.log("тип редактирования bool")
//                            }
//                            if(typeEdit == TypeEdit.DOUBLE_EDIT) {
//                                console.log("тип редактирования double")
//                            }
//                            if(typeEdit == TypeEdit.STRING_EDIT) {
//                                console.log("тип редактирования string")
//                            }
//                        }
//                    }

                }
            }
        }
    }
}
