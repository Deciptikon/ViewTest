import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
    property int lastNumItem: -1
    property int currentNumItem: -1

    anchors.fill: parent



    ListView {
        id: viewListField
        anchors {
            top: parent.top
            bottom: bottomMenu.top
            left: parent.left
            right: parent.right
        }
        clip: true // обрезка за пределами окна

        model: fieldModel
        delegate: Item {

            property bool activePress: false

            id: itemDelegateViewListField
            height: activePress ? actionDelegateViewListField.height + 50 : 50
            width: viewListField.width

            Rectangle {
                id: delegateViewListField

                height: 45
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: 5
                }
                radius: 10

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
                anchors.horizontalCenter: delegateViewListField.horizontalCenter
                color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
                visible: activePress
                Text {
                    id: textPreview
                    anchors.centerIn: parent
                    text: "P R E V I E W"
                    font.pixelSize: 40
                }
                Text {
                    id: textPathToData
                    text: pathToData
                    anchors.top: textPreview.bottom
                    anchors.horizontalCenter: textPreview.horizontalCenter
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
                radiusButton: 5

                textButton: qsTr("+ + +")

                colorPressed: Qt.rgba(0.5, 0.5, 0.5, 0.7)
                colorReleased: Qt.rgba(0.7, 0.7, 0.7, 0.7)

                onReleasedButton: {
                    //listField.append({name: "Added Field ++++++", size: "over 100500"})
                    // добавляем в базу данных
                    fieldDataBase.inserIntoTable("fnameField.text" + viewListField.count,
                                                 "232323",
                                                 "fields/" + viewListField.count + ".bin")

                    // обновляем модель для отображения данных из базы данных
                    fieldModel.updateModel()
                }
            }

            TextButton {
                id: btClear
                height: bottomMenu.height
                width: bottomMenu.width/2
                radiusButton: 5

                textButton: qsTr("...")

                colorPressed: Qt.rgba(0.5, 0.5, 0.5, 0.7)
                colorReleased: Qt.rgba(0.7, 0.7, 0.7, 0.7)

                onReleasedButton: {
                    //listField.clear()
                }
            }
        }
    }
}
