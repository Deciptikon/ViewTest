import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    anchors.fill: parent


    Rectangle {
        anchors.fill: parent
        anchors.margins: 5

        radius: 20

        color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

        clip: true

        Text {

            width: 200
            height: 200
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 16
            text: "Моя музыка - физика\n"   // этот текст либо оставить в qml и помечать qsTr()
                + "Симфонии - теоремы.\n"   // либо читать из c++ model и отмечать его там через QObject::tr()
                + "Моя жизнь не мистика,\n"
                + "А проблемы...\n"
                + "\n"
                + "Мой язык - математика,\n"
                + "Слова - формулы,\n"
                + "Аксиомы - грамматика,\n"
                + "Функции - глаголы.\n"
                + "\n"
                + "Мои мысли - понятия,\n"
                + "Чувства - категории.\n"
                + "Моя цель - понимание\n"
                + "И не более...\n"
                + "\n"
                + "Моё тело - Вселенная,\n"
                + "Разум - космос.\n"
                + "Всё бытие не тленное\n"
                + "Мой рождает Логос.\n"

        }

    }
}
