import QtQuick

Image {
    id: root
    antialiasing: true
    signal clicked

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
