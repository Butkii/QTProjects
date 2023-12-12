import QtQuick

Rectangle {
    width: 100; height: 100
    anchors.margins: 40
    property var tapped

    Repeater {
        model: 4
        delegate: Image {
            anchors { bottom: parent.bottom; bottomMargin: 30 }
            height: 40; width: 40
            source: "assets/arrow.svg"
            transformOrigin: Item.Bottom
            rotation: index * 90

            MouseArea {
                anchors.fill: parent
                onClicked: tapped(index)
            }
        }
    }
}
