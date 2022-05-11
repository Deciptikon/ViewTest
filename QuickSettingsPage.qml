import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    anchors.fill: parent

    StackView {
        id: stackViewQuickSettings
        anchors.fill: parent
        initialItem: "QuickSettingsOne.qml"
    }
}
