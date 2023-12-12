import QtQuick

Window {
    width: 640; height: 480
    visible: true
    title: qsTr("Hello World")

    Column {
        anchors { fill: parent; margins: 20 }

        Rectangle {
            id: rectBorder
            width: 500; height: 300
            border.color: "black"
            border.width: 3
            radius: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: box
                width: 40; height: 40
                color: "skyblue"
                anchors.centerIn: parent;
                states: [
                    State { name: "topAnchored"; AnchorChanges { target: box; anchors.top: rectBorder.top } },
                    State { name: "bottomAnchored"; AnchorChanges { target: box; anchors.bottom: rectBorder.bottom } },
                    State { name: "rightAnchored"; AnchorChanges { target: box; anchors.right: rectBorder.right } },
                    State { name: "leftAnchored"; AnchorChanges { target: box; anchors.left: rectBorder.left } }
                ]
                transitions: Transition { AnchorAnimation { duration: 200; easing.type: Easing.OutQuad } }
                Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.OutQuad } }
                Behavior on y { NumberAnimation { duration: 250; easing.type: Easing.OutQuad } }
            }
        }
        Row {
            spacing: 100
            anchors.horizontalCenter: parent.horizontalCenter

            PositionController {
                tapped: function(index) {
                    if(box.anchors.centerIn)
                        box.anchors.centerIn = undefined
                    box.state = ""
                    if (index === 0) {
                        if (box.y > 0)
                            box.y -= 10
                    } else if (index === 1) {
                        if (box.x < 460)
                            box.x += 10
                    } else if (index === 2) {
                        if (box.y < 260)
                            box.y += 10
                    } else if (index === 3) {
                        if (box.x > 0)
                            box.x -= 10
                    }
                }
            }

            PositionController {
                tapped: function(index) {
                    if(box.anchors.centerIn)
                        box.anchors.centerIn = undefined
                    if (index === 0) {
                        box.state = "topAnchored"
                    } else if (index === 1) {
                        box.state = "rightAnchored"
                    } else if (index === 2) {
                        box.state = "bottomAnchored"
                    } else if (index === 3) {
                        box.state = "leftAnchored"
                    }
                }
            }
        }
    }
}
