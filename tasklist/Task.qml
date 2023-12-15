import QtQuick

Rectangle {
    color: "lightgrey"
    border.color: "grey"
    border.width: 1
    radius: 8
    height: 50

    property var changeSelected
    property alias selectionMark: selectionMark
    property bool completed

    Row {
        spacing: 12
        anchors.centerIn: parent

        Rectangle {
            height: 14; width: 14
            radius: 7
            border.color: "black"
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                id: selectionMark
                height: 8; width: 8
                radius: 4
                anchors.centerIn: parent
                color: selected ? "black" : "white"

                MouseArea {
                    anchors.fill: parent
                    onClicked: { selected = !selected; console.log("ssss", selected) }
                }
            }
        }

        Column {
            spacing: 4
            width: 150
            Text {
                id: titleText
                text: title
                font.pixelSize: 16
                font.bold: true
                font.strikeout: completed
            }
            Text {
                id: desc
                text: description
                font.pixelSize: 12
                font.strikeout: completed
            }
        }

        Image {
            height: 14; width: 14
            anchors.verticalCenter: parent.verticalCenter
            source: completed ? "assets/checkbox-complete.svg" : "assets/checkbox.svg"
            antialiasing: true

            MouseArea {
                anchors.fill: parent;
                onClicked: completed = !completed
            }
        }
    }
}
