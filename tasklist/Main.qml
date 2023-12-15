import QtQuick
import MyListModel

Window {
    width: 250; height: 480
    visible: true
    title: qsTr("Hello World")
    property bool allSelected

    Item {
        id: listItem
        property var newTask
        anchors.fill: parent

        ListView {
            id: tasks
            anchors { top: header.bottom; bottom: parent.bottom } width: parent.width
            spacing: 10; clip: true; bottomMargin: 10
            model: MyListModel {}
            delegate: Task {
                x: 10; width: ListView.view.width-20
            }
        }

        Row {
            id: header
            anchors.horizontalCenter: parent.horizontalCenter
            height: 40; spacing: 24
            Rectangle {
                height: 14; width: 14
                radius: 7
                border.color: "black"
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    height: 10; width: 10
                    radius: 5
                    anchors.centerIn: parent
                    color: allSelected ? "black" : "transparent"

                    MouseArea {
                        anchors.fill: parent;
                        onClicked: {
                            allSelected = !allSelected
                            for (var i = 0; i < tasks.model.count(); i++) {
                                tasks.model.setProperty(i, "selected", allSelected)
                            }
                        }
                    }
                }
            }

            Text {
                text: "Tasks"
                font.pixelSize: 24
                font.bold: true
                color: "black"
                width: 140
            }

            Image {
                height: 14; width: 14
                anchors.verticalCenter: parent.verticalCenter
                source: "assets/delete.svg"
                antialiasing: true

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        for (var i = 0; i < tasks.model.count(); i++) {
                            if (tasks.model.get(i).selected) {
                                tasks.model.remove(i);
                                i--; allSelected = false;
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            height: 40; width: 40
            radius: 20
            color:"black"
            anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 20 }
            Text {
                text: "+"
                font.pixelSize: 24
                color: "white"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    const comp = Qt.createComponent("NewTaskView.qml").createObject(listItem, {submit: (title, desc)=>tasks.model.append(title, desc, false)})
                }
            }
        }
    }
}
