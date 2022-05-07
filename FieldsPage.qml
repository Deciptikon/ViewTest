import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
    property int lastNumItem
    property int currentNumItem

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
            active: false
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
        anchors.fill: parent
        model: listField
        delegate: Item {
            property bool activePress: false

            id: itemDelegateViewListField
            height: activePress ? 150 : 50
            width: parent.width
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

                Column {
                    id: textColumn
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.topMargin: 5

                    Text {
                        text: "Name: " + name
                        font.pixelSize: 16
                    }
                    Text {
                        text: "Size: " + size + " Acre"
                        font.pixelSize: 16
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        delegateViewListField.color = Qt.rgba(0.7, 0.7, 0.7, 0.9)
                    }
                    onReleased: {
                        delegateViewListField.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
                        console.log(index)

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
                            viewListField.itemAtIndex(lastNumItem).activePress = false
                            activePress = true
                            lastNumItem = currentNumItem
                        }

//                        viewListField.itemAtIndex(numItem).activePress = false
//                        numItem = index
//                        activePress = true

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
            }

        }
    }
}
