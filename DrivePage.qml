import QtQuick 2.0

import DrawTrack 1.0

Item {
    anchors.fill: parent


    Connections {
            // описываем реакцию на испущенные сигналы из слоя modelView
            target: modelView

            //получаем и\или удаляем точки траектории для отображения в QML
            function onSignalAppPointToPathQML(point) {
                drawtrack.appPointToPath(point)
            }
            function onSignalAppPointToPathAndRemoveFirstQML(point) {
                drawtrack.appPointToPathAndRemoveFirst(point)
            }

            //ролучаем список ключевых точек для отображения в QML
            function onKeyPointsToQML(keypoints) {
                drawtrack.updateKeyPoint(keypoints)
            }

        }

    Connections {
            // описываем реакцию на испущенные сигналы из итема drawtrack
            target: drawtrack

            function onReleaseCoordinate(point) {

                // передаем координаты ключевой точки в слой modelView
                // который потом передаст их в автопилот, а тот
                // в свою очередь обработает их и добавит в конец списка
                // ключевых точек, и испустит сигнал с новым
                // списком ключевых точек, которые получит слой ViewData
                // и передаст их для отображения в QML drawtrack
                modelView.addKeyPointFromQML(point)
            }
        }

    DrawTrack {
            id: drawtrack
            anchors.fill: parent
            anchors.centerIn:  parent

            //проверка работы параметров
            msecUpdate: 1000
            isUpdateFromChanged: true
            widthPath: 3
            colorPath: "#ff0000"
            colorKeyPoint: "#0000ff"
            colorGround: "#aaffaa"
            isPaintAxis: true
        }

       Rectangle {//итем изменения масштаба
           id: zoomItem
           anchors.right:  parent.right
           anchors.top:  parent.top
           anchors.rightMargin: 15
           anchors.topMargin: 25
           color: "#ddffff"
           width: 80
           height: 240

           Text {//текущая величина масштаба
               id: currentZoom
               anchors.centerIn: parent
               text: drawtrack.getZoom()
               font.pixelSize: 25
           }

           TextButton {//приближение
               id: zoomIn
               anchors.top:  parent.top
               anchors.horizontalCenter: parent.horizontalCenter
               width: parent.width
               height: parent.height/3
               textButton: "+"
               sizeTextButton: 40
               onReleasedButton: {
                   drawtrack.zoomIn()
                   currentZoom.text = drawtrack.getZoom()
               }
           }

           TextButton {//отдаление
               id: zoomOut
               anchors.bottom:  parent.bottom
               anchors.horizontalCenter: parent.horizontalCenter
               width: parent.width
               height: parent.height/3
               textButton: "-"
               sizeTextButton: 50
               onReleasedButton: {
                   drawtrack.zoomOut()
                   currentZoom.text = drawtrack.getZoom()
               }
           }
       }

       ImageButton {//кнопка переключения центрирования (ЗАМЕНИТЬ НА ЧЕКБОКС)
              id: swapbutton
              width: zoomItem.width
              height: width

              anchors.top: zoomItem.bottom
              anchors.topMargin: 25
              anchors.horizontalCenter: zoomItem.horizontalCenter
              anchors.horizontalCenterOffset: 0

              sourcePressed: "qrc:/image/bt-centered-false.png"
              sourceReleased: "qrc:/image/bt-centered-true.png"

              onReleasedButton: drawtrack.swapCentered()

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
//            text: qsTr("Drive Page")
//            color: "black"
//            font.pixelSize: 40
//        }
//    }
}
