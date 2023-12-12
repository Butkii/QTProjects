import QtQuick
import QtQuick.Window

Window {
    width: 500; height: 600
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        color: "lightblue"
        radius: 10
        anchors.centerIn: parent
        scale: Math.min(parent.height/parent.width, parent.width/parent.height)
        width: col.width + 60; height: col.height + 60
//        width: Math.min(height, col.width); height: Math.min(460, Math.min(parent.height, parent.width)-60)
        //anchors { fill: parent; topMargin: 30; bottomMargin: 30; leftMargin: 80; rightMargin: 80}

        Column {
            id: col
            width: 360
            y: 30
            spacing: 60
            property bool opPressed: false

            Rectangle {
                id: display
                color: "white"
                radius: 10
                anchors.horizontalCenter: parent.horizontalCenter
                height: 100; width: 300
                property string displayText: ""
                property int num1
                property int num2
                property string op

                Text{
                    text: display.num1
                    anchors { right: parent.right; rightMargin: 15; verticalCenter: parent.verticalCenter}
                    color: "grey"
                }

                Text {
                    text: display.displayText
                    anchors { right: parent.right; rightMargin: 15; verticalCenter: parent.verticalCenter; verticalCenterOffset: 16}
                    font.pixelSize: 20
                }
            }

            Grid {
                id: gri
                anchors { left: parent.left; right: parent.right; margins: 20}
                spacing: 20
                Repeater {
                    model: ["7", "8", "9", "/", "4", "5", "6", "*", "1", "2", "3", "-", "0", "C", ".", "+"]
                    delegate: Rectangle {
                        height: 80; width: 80
                        radius: 8
                        color: modelData.match(/\d/) ? "white" : "orange"

                        Text {
                            text: modelData
                            color: "black"
                            anchors.centerIn: parent
                            font.bold: true
                            font.pixelSize: 16
                        }

                        MouseArea {
                            anchors.fill: parent;
                            onClicked: function() {
//                                if (type === "op") {
//                                    display.op = buttonText
//                                    display.num2 = display.num1 ? display.num1 : 0
//                                    display.num1 = parseInt(display.displayText)
//                                    display.displayText = ""
//                                } else {
//                                    display.displayText += buttonText
//                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
