import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
    property int lastNumItem: -1
    property int currentNumItem: -1

    anchors.fill: parent

    ListModel {
        id: listField

        ListElement{
            name: "Field 1"
            size: "132.00"
        }
        ListElement{
            name: "Field 2"
            size: "1052.3"
        }
        ListElement{
            name: "Test field"
            size: "95.23"
        }
        ListElement{
            name: "Old field"
            size: "8462.5"
        }
        ListElement{
            name: "The best field"
            size: "1578.65"
        }
        ListElement{
            name: "Test 2022"
            size: "435.43"
        }
        ListElement{
            name: "Test 4545"
            size: "33.433"
        }
        ListElement{
            name: "Test 42"
            size: "952.323"
        }
        ListElement{
            name: "Test 999"
            size: "10000.00"
        }
        ListElement{
            name: "Test 666"
            size: "большое поле"
        }
        ListElement{
            name: "Test 777"
            size: "777.77"
        }
        ListElement{
            name: "Test 20"
            size: "32.323"
        }
    }

    ListView {
        id: viewListField
        anchors {
            top: parent.top
            bottom: bottomMenu.top
            left: parent.left
            right: parent.right
        }

        model: fieldModel//listField
        delegate: Item {

            property bool activePress: false

            id: itemDelegateViewListField
            height: activePress ? actionDelegateViewListField.height + 50 : 50
            width: viewListField.width
            anchors {
                leftMargin: 5
                rightMargin: 5
                topMargin: 5
                bottomMargin: 5
            }

            Rectangle {
                id: delegateViewListField
                height: 45
                width: parent.width
                radius: 10
                anchors.top: parent.top
                anchors.topMargin: 5

                color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

                TextButton {
                    id: btDel
                    height: parent.height - 5
                    width: 60
                    radiusButton: 5

                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter

                    textButton: qsTr("delete")
                    colorTextButton: Qt.rgba(1, 1, 1, 1)

                    colorPressed: Qt.rgba(0.5, 0.5, 0.5, 0.9)
                    colorReleased: Qt.rgba(0.3, 0.3, 0.3, 0.9)

                    onReleasedButton: {

                        /// здесь есть баг))))
                        currentNumItem = index
                        if(currentNumItem < lastNumItem) {
                            if(currentNumItem == lastNumItem - 1) {
                                --lastNumItem
                            } else {
                                viewListField.itemAtIndex(lastNumItem).activePress = false
                                viewListField.itemAtIndex(--lastNumItem).activePress = true
                            }
                        } else if (currentNumItem == lastNumItem) {
                            viewListField.itemAtIndex(lastNumItem).activePress = false
                            lastNumItem = -1
                        }

                        //listField.remove(index)
                        fieldDataBase.removeRecord(fieldModel.getId(currentNumItem))
                        fieldModel.updateModel();  // Обновляем модель данных
                    }
                }

                Column {
                    id: textColumn
                    width: parent.width/2
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    //anchors.topMargin: 5

                    Text {
                        text: "Name: " + name
                        font.pixelSize: 16
                    }
                    Text {
                        text: "Size: " + size + " m2"
                        font.pixelSize: 16
                    }
                }

                MouseArea {
                    //anchors.fill: parent
                    anchors {
                        left: parent.left
                        right: btDel.left
                        top: parent.top
                        bottom: parent.bottom
                    }

                    onPressed: {
                        delegateViewListField.color = Qt.rgba(0.7, 0.7, 0.7, 0.9)
                    }
                    onReleased: {
                        delegateViewListField.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
                        console.log(index, pathToData)

                        currentNumItem = index
                        if(lastNumItem < 0) {
                            activePress = true
                            lastNumItem = currentNumItem
                            return
                        }

                        if(lastNumItem == currentNumItem) {
                            activePress = false
                            lastNumItem = currentNumItem = -1
                        } else {
                            if(lastNumItem >= viewListField.count) {
                                activePress = true
                                lastNumItem = currentNumItem
                                return
                            }
                            viewListField.itemAtIndex(lastNumItem).activePress = false
                            activePress = true
                            lastNumItem = currentNumItem
                        }
                    }
                    onCanceled: {
                        delegateViewListField.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
                    }
                }
            }

            Rectangle {
                id: actionDelegateViewListField
                width: delegateViewListField.width
                height: 100
                radius: 10
                anchors.top: delegateViewListField.bottom
                color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
                visible: activePress
                Text {
                    anchors.centerIn: parent
                    text: "P R E V I E W"
                    font.pixelSize: 40
                }
            }
        }
    }

    Item {
        id: bottomMenu
        width: parent.width
        height: 50
        anchors.bottom: parent.bottom

        Row {

            TextButton {
                id: btAdd
                height: bottomMenu.height
                width: bottomMenu.width/2

                textButton: qsTr("+ + +")

                colorPressed: Qt.rgba(0.5, 0.5, 0.5, 1)
                colorReleased: Qt.rgba(0.7, 0.7, 0.7, 1)

                onReleasedButton: {
                    //listField.append({name: "Added Field ++++++", size: "over 100500"})
                    fieldDataBase.inserIntoTable("fnameField.text" + viewListField.count,
                                                 "232323",
                                                 "fields/" + viewListField.count + ".bin")
                    fieldModel.updateModel()
                }
            }

            TextButton {
                id: btClear
                height: bottomMenu.height
                width: bottomMenu.width/2

                textButton: qsTr("...")

                colorPressed: Qt.rgba(0.5, 0.5, 0.5, 1)
                colorReleased: Qt.rgba(0.7, 0.7, 0.7, 1)

                onReleasedButton: {
                    //listField.clear()
                }
            }
        }
    }
}
