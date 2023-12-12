import QtQuick

Rectangle {
    width: 100
    height: 100
    anchors {
        margins: 40
    }

    ClickableImage {
        anchors {
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: 10
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 10
        }

        height: 40
        width: 40

        source: "assets/arrow.svg"
        onClicked: {

        }
    }

    ClickableImage {
        anchors {
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: 40
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 40
        }

        height: 40
        width: 40
        rotation: 90

        source: "assets/arrow.svg"
        onClicked: {
            rect.anchors.left = undefined
            rect.anchors.right = rectBorder.right
        }
    }

    ClickableImage {
        anchors {
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: 10
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 70
        }

        height: 40
        width: 40
        rotation: 180

        source: "assets/arrow.svg"
        onClicked: {
            rect.anchors.top = undefined
            rect.anchors.bottom = rectBorder.bottom
        }
    }

    ClickableImage {
        anchors {
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: -20
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 40
        }

        height: 40
        width: 40
        rotation: 270

        source: "assets/arrow.svg"
        onClicked: {
            rect.anchors.right = undefined
            rect.anchors.left = rectBorder.left
        }
    }
}
