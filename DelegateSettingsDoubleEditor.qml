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
    DoubleValueEditor {
        id: dbEdit
        width: 120
        height: parent.height - 5
        radiusDoubleEditor: 5

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        maxValueDouble: 1000
        stepEditValueDouble: 10
        currentValueDouble: value

//        visible: isDoubleEdit
//        enabled: isDoubleEdit

        onMyCurrentValueDoubleChanged: {
            value = currentValueDouble
        }
        // здесь можно добавить кнопку установки значения,
        // и перенести в неё код из onMyCurrentValueDoubleChanged
        // если не нужно сохранять настройки при каждом изменении
    }
}
