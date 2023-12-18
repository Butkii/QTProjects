import QtQuick
import QtQuick.Window
import MyListModel

Window {
    width: 640; height: 480
    visible: true
    title: qsTr("Contact List")

    Item {
        anchors.fill: parent

        Text {
            id: header
            height: 50
            anchors {
                top: parent.top; topMargin: 25
                horizontalCenter: parent.horizontalCenter
            }
            text: "Contacts"
            font { pixelSize: 24; bold: true }
        }

        ListView {
            anchors { top: header.bottom; bottom: parent.bottom; }
            width: parent.width
            model: MyListModel {}
            spacing: 30
            delegate: Rectangle {
                height: 50; width: parent.width
                anchors { left: parent.left; leftMargin: 20 }
                radius: 10
                Column {
                    spacing: 8
                    Text {
                        text: name
                        font.pixelSize: 18
                        font.bold: true
                    }
                    Text {
                        text: phoneNumber
                        font.pixelSize: 16
                    }
                }
            }
        }
    }
}
