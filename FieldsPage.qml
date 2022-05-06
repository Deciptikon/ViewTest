import QtQuick 2.0
import QtQuick.Layouts 1.12

Item {
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
        anchors.fill: parent

        model: listField

        delegate: Item {
                    id: itemDelegateViewListField

                    height: 50


                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 5
                        rightMargin: 5
                        topMargin: 5
                        bottomMargin: 5
                    }

            Rectangle {
                id: delegateViewListField
                height: parent.height - 5
                width: parent.width
                radius: 10
                anchors.centerIn: parent

                color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

                Column {
                    anchors.fill: parent
                    anchors.leftMargin: 10

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
                    }
                    onCanceled: {
                        delegateViewListField.color = Qt.rgba(0.7, 0.7, 0.7, 0.7)
                    }
                }
            }
        }
    }

//    Rectangle {
//        width: parent.width/2
//        height: parent.height/2
//        radius: parent.width/20
//        anchors.centerIn: parent
//        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)
//        Text {
//            id: testText
//            anchors.centerIn: parent
//            text: qsTr("Fields Page")
//            color: "black"
//            font.pixelSize: 40
//        }
//    }
}
