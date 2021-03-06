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

            //ролучаем список ключевых точек для отображения в QML
            function onKeyPointsToQML(keypoints) {
                drawtrack.updateKeyPoint(keypoints)
            }

            //добавляем точку А
            function onSendPointAToQML(point) {
                drawtrack.setPointA(point)
            }

            //добавляем направление параллельного вождения
            function onSendDirectToQML(dir) {
                drawtrack.setDirect(dir)
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
            lengthPath: 1500
            widthFlat: 0
            colorFlat: "#ffff00"
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

       ImageCheckBox {
           id: swapIsCenteredLastPoint
           width: zoomItem.width
           height: width

           anchors.top: zoomItem.bottom
           anchors.topMargin: 25
           anchors.horizontalCenter: zoomItem.horizontalCenter
           anchors.horizontalCenterOffset: 0

           sourceTrue: "qrc:/image/bt-centered-false.png"
           sourceFalse: "qrc:/image/bt-centered-true.png"

           value: drawtrack.isCenteredLastPoint

           onChangedImageCheckBox: {
               console.log("onChangedImageCheckBox")
               console.log(swapIsCenteredLastPoint.value)
               //drawtrack.swapCentered()
               //swapcheckbox.value = drawtrack.isCenteredLastPoint
               drawtrack.isCenteredLastPoint = swapIsCenteredLastPoint.value
           }
       }

       ImageCheckBox {
           id: swapRelay
           width: zoomItem.width
           height: width

           anchors.top: swapIsCenteredLastPoint.bottom
           anchors.topMargin: 25
           anchors.horizontalCenter: zoomItem.horizontalCenter
           anchors.horizontalCenterOffset: 0

           sourceFalse: "qrc:/image/checkbox-state-off.png"
           sourceTrue: "qrc:/image/checkbox-state-on.png"

           value: false

           onChangedImageCheckBox: {
               console.log("Restate Relay")
               modelView.swapRelayState()
               // можно было бы вызывать эту функцию с типом Boolean
               // для того что бы узнать статус операции по отправке данных
               // и обновить состояние переключателя в зависимости от него
           }
       }


       ButtonAB {
           id: buttonAB
           width: zoomItem.width
           height: width

           anchors.bottom: parent.bottom
           anchors.bottomMargin: 25
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.horizontalCenterOffset: 0

           sourceA: "qrc:/image/bt-letter-a.png"
           sourceB: "qrc:/image/bt-letter-b.png"

           visible: modelView.isVisibleButtonAB()
           enabled: modelView.isEnableButtonAB()

           onChangedButtonAB: {
               if(buttonAB.value == 2) {
                   console.log("Set point A")
                   modelView.signalSetPointA()
               }
               if(buttonAB.value == 3) {
                   console.log("Set point B")
                   modelView.signalSetPointB()
               }
               //modelView.slotSendMsgToSlave14(14)
           }
       }
}
