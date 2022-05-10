import QtQuick 2.0
import QtQuick.Layouts 1.12

import TypeEdit 1.0 // ENUM способ редактирования настроек

Item {
    anchors.fill: parent

    ListView {
        id: viewListSettings
        anchors.fill: parent
        clip: true // обрезка за пределами окна

        model: appSettings
        delegate: Item {
            id: itemDelegateSettings
            height: 50
            width: viewListSettings.width

            Rectangle {
                radius: 10
                anchors.fill: parent
                anchors.topMargin: 5
                anchors.leftMargin: 5
                anchors.rightMargin: 5

                color: Qt.rgba(0.7, 0.7, 0.7, 0.7)

                Loader {
                    id: loaderSettings
                    anchors.fill: parent
                    source: switch(typeEdit) {
                            case TypeEdit.NONE_EDIT:   return "DelegateSettingsNoneEditor.qml"
                            case TypeEdit.BOOL_EDIT:   return "DelegateSettingsBoolEditor.qml"
                            case TypeEdit.DOUBLE_EDIT: return "DelegateSettingsDoubleEditor.qml"
                            case TypeEdit.STRING_EDIT: return "DelegateSettingsStringEditor.qml"
                            }
                }
            }
        }
    }
}


