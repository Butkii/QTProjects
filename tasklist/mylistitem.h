#ifndef MYLISTITEM_H
#define MYLISTITEM_H

#include <QObject>
#include <QVector>

class MyListItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY (QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY (bool selected READ selected WRITE setSelected NOTIFY selectChanged)
public:
    MyListItem(const QString& title, const QString& description, bool selected)
        :m_title(title), m_description(description), m_selected(selected){}
    MyListItem() = default;
    MyListItem(const MyListItem& other);

    const QString& title() const;
    void setTitle(const QString& title);

    const QString& description() const;
    void setDescription(const QString& description);

    bool selected() const;
    void setSelected(bool selected);

    MyListItem& operator=(const MyListItem& other);

private:
    QString m_title;
    QString m_description;
    bool m_selected;

signals:
    void titleChanged(QString);
    void descriptionChanged(QString);
    void selectChanged(bool);
};
Q_DECLARE_METATYPE(MyListItem)

#endif // MYLISTITEM_H
