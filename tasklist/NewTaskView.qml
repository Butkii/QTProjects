import QtQuick

Rectangle {
    id: root
    width: parent.width; height: parent.height
    anchors.horizontalCenter: parent.horizontalCenter
    property var submit

    Column {
        topPadding: 15
        spacing: 10
        width: 230
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            spacing: 20
            Image {
                source: "assets/back.png"
                height:20; width: 20;
                MouseArea {
                    anchors.fill: parent
                    onClicked: root.destroy()
                }
            }

            Text {
                text: "New Task"
                font.pixelSize: 24
                font.bold: true
            }
        }

        TextEdit {
            id: title
            font.pixelSize: 16
            font.bold: true
            width: parent.width
            Text {
                text: "Title"
                color: "#aaa"
                visible: !title.text
                anchors.left: title.left
            }
        }

        TextEdit {
            id: description
            font.pixelSize: 12
            height: 150;width: parent.width

            Text {
                text: "Description"
                color: "#aaa"
                visible: !description.text
                anchors.left: description.left
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
                    submit(title.text, description.text)
                    root.destroy()
                }
            }
        }
    }
}
