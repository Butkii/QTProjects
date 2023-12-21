import QtQuick

Rectangle {
    id: root
    width: parent.width; height: parent.height
    anchors.horizontalCenter: parent.horizontalCenter
    property var submit
    property alias nameText: name.text
    property alias numberText: phoneNumber.text

    Column {
        topPadding: 15
        spacing: 40
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            spacing: 80
            anchors { horizontalCenter: parent.horizontalCenter; top: root.bottom; topMargin: 80 }

            Image {
                source: "assets/back.png"
                height:25; width: 25;
                MouseArea {
                    anchors.fill: parent
                    onClicked: root.destroy()
                }
            }

            Text {
                text: "New Contact"
                font { pixelSize: 24; bold: true }
            }
        }

        TextInput {
            id: name
            font.pixelSize: 18
            width: root.width - 100
            visible: editable
            maximumLength: 30
            Text {
                text: "Name"
                color: "#aaa"
                visible: !name.activeFocus && name.text.length === 0
                anchors.left: name.left
                font.pixelSize: 18
            }
        }

        TextInput {
            id: phoneNumber
            font.pixelSize: 18
            height: 150; width: root.width - 100
            visible: editable
            inputMethodHints: Qt.ImhDigitsOnly
            maximumLength: 15
            Text {
                text: "PhoneNumber"
                color: "#aaa"
                visible: !phoneNumber.activeFocus && phoneNumber.text.length === 0
                anchors.left: phoneNumber.left
                font.pixelSize: 18
            }
        }

        Rectangle {
            height: 30; width: 100; radius: 20
            anchors.horizontalCenter: parent.horizontalCenter
            border.color:"black"
            Text {
                text: "Save"
                font.pixelSize: 16
                font.bold: true
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    submit(name.text, phoneNumber.text)
                    root.destroy()
                }
            }
        }
    }
}
