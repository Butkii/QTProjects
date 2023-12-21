#include "contactlist.h"

#include <QDebug>
#include <QJNIObject>

ContactList::ContactList(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Getting contacts...";

    auto contactslist = QJniObject::callStaticObjectMethod<jstring>("com/contactlist/MainActivity", "getContactList").toString();
    qDebug() << contactslist;

    setContacts(contactslist);
}

const QString& ContactList::contacts() const { return m_contacts; }

void ContactList::setContacts(const QString& contactList) {
    m_contacts = contactList;
    emit contactListChanged();
}
