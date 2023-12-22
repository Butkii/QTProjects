import QtQuick
import QtQuick.Window
import ContactListModel

Window {
    width: 640; height: 480
    visible: true
    title: qsTr("Contact List")
    property int contactsSelected: 0

    Item {
        id: root
        anchors.fill: parent

        Item {
            id: list
            anchors.fill: parent

            Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.OutQuad } }
            states: [
                State {
                    name: "slideIn"
                    PropertyChanges { target: list; x: 0 }
                },
                State {
                    name: "slideOut"
                    PropertyChanges { target: list; anchors.fill: ""; x: -600 }
                }
            ]

            Row {
                id: header
                height: 50; spacing: 80
                anchors { top: parent.top; topMargin: 25; horizontalCenter: parent.horizontalCenter }

                Text {
                    text: "Contacts"
                    font { pixelSize: 24; bold: true }
                }

                Rectangle {
                    height: 40; width: 40; radius: 20
                    color: "lightgrey"

                    Image {
                        height: 25; width: 25
                        source: "assets/delete.svg"
                        antialiasing: true
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent;
                        onClicked: {
                            if (contactsSelected > 0) {
                                list.opacity = 0.3
                                Qt.createComponent("Popup.qml").createObject(root,  { cancel: function() {
                                    list.opacity = 1
                                    contacts.model.setProperty("selected", false)
                                    contactsSelected = 0
                                }, confirm: function() {
                                    list.opacity = 1
                                    for (var i = 0; i < contacts.model.count(); i++) {
                                        if (contacts.model.get(i).selected) {
                                            contacts.model.remove(contacts.model.get(i).phoneNumber)
                                            i--
                                        }
                                    }
                                }})
                            }
                        }
                    }
                }
            }

            ListView {
                id: contacts
                anchors { top: header.bottom; bottom: parent.bottom }
                clip: true; spacing: 30
                width: parent.width; bottomMargin: 20
                model: ContactListModel {}
                delegate: Rectangle {
                    height: 50; width: parent.width; radius: 10
                    anchors { left: parent.left; leftMargin: 20}

                    MouseArea {
                        anchors.fill: parent;
                        onClicked:  {
                            list.state = "slideOut"
                            const comp = Qt.createComponent("EditContactView.qml").createObject(
                                       root, {
                                           nameText: name, numberText: phoneNumber,
                                           submit: (newName, newNumber)=>{ contacts.model.update(newName, newNumber) }
                                       })
                        }
                    }

                    Row {
                        spacing: 20

                        Rectangle {
                            anchors { verticalCenter: parent.verticalCenter; verticalCenterOffset: 10}
                            height: 48; width: 48; radius: 24
                            color: selected ? "lightgreen" : "lightblue"

                            Text {
                                anchors.centerIn: parent
                                text: selected ? "/" : section
                                font { pixelSize: 24; bold: true }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    selected ? contactsSelected-- : contactsSelected++
                                    contacts.model.setProperty(index, "selected", !selected)
                                }
                            }
                        }

                        Text {
                            anchors { verticalCenter: parent.verticalCenter; verticalCenterOffset: 10}
                            text: "<b>" + name + "</b><br>" + phoneNumber
                            font.pixelSize: 18
                        }
                    }

                }

                section.criteria: ViewSection.FullString
                section.property: "section"
                section.delegate: Rectangle {
                    width: ListView.view.width; height: 35
                    color: "lavender"
                    Text {
                        padding: 5; leftPadding: 15
                        text: section
                        font { bold: true; pixelSize: 20 }
                    }
                }

            }

            Rectangle {
                height: 50; width: 50; radius: 25
                color:"black"
                anchors { right:parent.right; rightMargin: 20; bottom: parent.bottom; bottomMargin: 20 }
                Text {
                    text: "+"
                    font.pixelSize: 24
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.createComponent("NewContactView.qml").createObject(root, {submit: (name, number)=>{contacts.model.add(name, number)}})
                }
            }
        }
    }
}
