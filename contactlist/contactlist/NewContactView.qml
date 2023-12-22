import QtQuick

Rectangle {
    id: root
    width: parent.width; height: parent.height
    x: 800
    property var submit
    property alias nameText: name.text
    property alias numberText: phoneNumber.text
    MouseArea { anchors.fill: parent }
    Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.OutQuad } }
    states: [
        State {
            name: "slideIn"
            PropertyChanges { target: root; x: 0 }
        },
        State {
            name: "slideOut"
            PropertyChanges { target: root; x: 600 }
        }
    ]

    Component.onCompleted: { root.state = "slideIn" }

    Timer {
        id: destroy; interval: 300; running: false; repeat: false
        onTriggered: root.destroy()
    }

    Column {
        topPadding: 15
        spacing: 40
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            spacing: 80
            anchors { horizontalCenter: parent.horizontalCenter; top: root.bottom; topMargin: 80 }

            Image {
                source: "assets/back.png"
                height: 25; width: 25
                MouseArea {
                    anchors.fill: parent
                    onClicked: { root.state = "slideOut"; destroy.running = true }
                }
            }

            Text {
                text: "New Contact"
                font { pixelSize: 24; bold: true }
            }
        }

        TextEdit {
            id: name
            font.pixelSize: 18
            width: root.width - 100

            Text {
                text: "Name"
                color: "#aaa"
                visible: !name.activeFocus && name.text.length === 0
                anchors.left: name.left
                font.pixelSize: 18
            }
        }

        TextEdit {
            id: phoneNumber
            font.pixelSize: 18
            height: 150; width: root.width - 100
            inputMethodHints: Qt.ImhDigitsOnly

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
                    phoneNumber.text = phoneNumber.preeditText
                    phoneNumber.focus = false
                    name.text = name.preeditText
                    name.focus = false
                    submit(name.text, phoneNumber.text)
                    root.state = "slideOut"; destroy.running = true
                }
            }
        }
    }
}
