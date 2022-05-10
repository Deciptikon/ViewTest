import QtQuick 2.0
import QtQuick.Layouts 1.12

import TypeEdit 1.0 // способ редактирования настроек

Item {
    anchors.fill: parent

    ListModel {
        id: listSettings

        ListElement{
            name: "Option1"
            size: "132.00"
        }
        ListElement{
            name: "Option2"
            size: "51"//appSettings.getValueString("Option2")
        }
        ListElement{
            name: "Option3"
            size: "95.23"
        }
        ListElement{
            name: "Option4"
            size: "8462.5"
        }
        ListElement{
            name: "Option5"
            size: "1578.65"
        }
    }

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


