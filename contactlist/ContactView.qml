import QtQuick

Rectangle {
    id: root
    width: parent.width; height: parent.height; x: 800
    property var submit
    property alias nameText: name.text
    property alias numberText: phoneNumber.text
    property bool editable: false
    MouseArea { anchors.fill: parent }

    Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.OutQuad } }

    states: [
        State { name: "slideIn"; PropertyChanges { target: root; x: 0 } },
        State { name: "slideOut"; PropertyChanges { target: root; x: 800 } }
    ]

    Component.onCompleted: {
       root.state = "slideIn"
    }

    Timer {
        id: destroy; interval: 400; running: false; repeat: false
        onTriggered: root.destroy()
    }

    Column {
        topPadding: 15; spacing: 40
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            spacing: 80
            anchors { horizontalCenter: parent.horizontalCenter }

            Image {
                source: "assets/back.png"
                height: 25; width: 25;
                MouseArea {
                    anchors.fill: parent
                    onClicked: { root.state = "slideOut"; destroy.running = true }
                }
            }

            Text {
                text: editable ? "Contact Details" : "Contact Info"
                font { pixelSize: 24; bold: true }
            }

            Rectangle {
                height: 40; width: 40; radius: 20
                color: editable ? "lightgrey" : "white"
                border { color: editable ? "white" : "lightgrey"; width: 2 }

                Image {
                    height: 25; width: 25
                    source: "assets/edit.svg"
                    antialiasing: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        editable = !editable
                        if (!editable) { name.focus = false; phoneNumber.focus = false }
                    }
                }
            }
        }

        Text {
            text: nameText
            font.pixelSize: 18
            font.bold: true
            width: root.width - 100
            visible: !editable
        }

        TextEdit {
            id: name
            font.pixelSize: 18
            width: root.width - 100
            visible: editable

            Text {
                text: "Name"
                color: "#aaa"; font.pixelSize: 18
                visible: !name.activeFocus && name.text.length === 0
                anchors.left: name.left
            }
        }

        Text {
            text: numberText
            font { pixelSize: 18; bold: true }
            height: 150; width: root.width - 100
            visible: !editable
        }

        TextEdit {
            id: phoneNumber
            font.pixelSize: 18
            height: 150; width: root.width - 100
            visible: editable
            inputMethodHints: Qt.ImhDigitsOnly
            onTextChanged: if (numberText.length > 15) { numberText = numberText.slice(0,15); cursorPosition = numberText.length }
            Text {
                text: "PhoneNumber"
                color: "#aaa"; font.pixelSize: 18
                visible: !phoneNumber.activeFocus && phoneNumber.text.length === 0
                anchors.left: phoneNumber.left
            }
        }

        Rectangle {
            height: 30; width: 100; radius: 20
            anchors.horizontalCenter: parent.horizontalCenter
            border.color:"black"
            visible: editable
            Text {
                text: "Save"
                font { pixelSize: 16; bold: true }
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    phoneNumber.focus = false; name.focus = false
                    submit(name.text, phoneNumber.text)
                    root.state = "slideOut"; destroy.running = true
                }
            }
        }
    }
}
