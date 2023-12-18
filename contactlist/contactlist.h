#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QObject>

class ContactList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString contacts READ contacts WRITE setContacts NOTIFY contactListChanged)

public:
    explicit ContactList(QObject *parent = nullptr);

    const QString& contacts() const;
    void setContacts(const QString& contactList);

    ContactList& operator=(const ContactList& other);

private:
    QString m_contacts;

signals:
    void contactListChanged();
};

#endif // CONTACTLIST_H
