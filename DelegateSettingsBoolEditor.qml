import QtQuick 2.0

Item {
    anchors.fill: parent

    Text {
        text: title
        font.pixelSize: 16
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }





//    TextButton {
//        id: btTxtEdt

//        property string strBool: value
//        property bool stt: strBool == "true"

//        width: height
//        height: parent.height - 5

//        anchors.right: parent.right
//        anchors.rightMargin: 10
//        anchors.verticalCenter: parent.verticalCenter

//        radiusButton: 5

//        textButton: ""

//        colorReleased: stt ? Qt.rgba(0.2, 0.2, 0.2, 1) : Qt.rgba(0.5, 0.5, 0.5, 0.2)
//        colorPressed: Qt.rgba(0.7, 0.7, 0.7, 1)

//        colorBorder: Qt.rgba(0.2, 0.2, 0.2, 1)
//        widthBorderButton: 2

//        onReleasedButton: {
//            console.log("stt =" + stt)
//            if(stt) {
//                value = "false"
//                strBool = value
//                console.log("true --> false")
//            } else {
//                value = "true"
//                strBool = value
//                console.log("false --> true")
//            }

//            stt = strBool == "true"
//            console.log("stt =" + stt)
//            colorReleased = stt ? Qt.rgba(0.2, 0.2, 0.2, 1) : Qt.rgba(0.5, 0.5, 0.5, 0.2)
//            console.log("colorReleased =" + colorReleased)
//        }

//    }




    BoolValueEditior {
        id: bEdit
        width: height
        height: parent.height - 5

        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        currentStateBool: value

        onMyCurrentStateBoolChanged: {
            value = currentStateBool
            console.log("value =" + value)
        }
    }
}
