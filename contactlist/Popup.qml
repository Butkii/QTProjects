import QtQuick

Rectangle {
    id: root
    property var confirm
    property var cancel
    anchors.fill: parent
    color: "transparent"
    MouseArea {
        anchors.fill: parent
        onClicked: { root.destroy(); cancel(); }
    }

    Rectangle {
        width: 500; height: 200; radius: 40
        anchors.centerIn: parent
        border { color: "lightgrey"; width: 2 }
        MouseArea{ anchors.fill: parent }

        Text {
            text: "Are you sure you want to delete?"
            anchors{ top: parent.top; topMargin: 40; horizontalCenter: parent.horizontalCenter }
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
        }

        Row {
            anchors{ bottom: parent.bottom; bottomMargin: 30; horizontalCenter: parent.horizontalCenter }
            spacing: 20

            Rectangle {
                width: 75; height: 50; radius: 10
                border { color: "black"; width: 2 }

                Text {
                    text: "Cancel"
                    font.pixelSize: 16
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: { root.destroy(); cancel(); }
                }
            }

            Rectangle {
                width: 75; height: 50; radius: 10
                border { color: "black"; width: 2 }
                color: "lightskyblue"

                Text {
                    text: "Confirm"
                    font.pixelSize: 16
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: { confirm(); root.destroy() }
                }
            }
        }
    }
}
