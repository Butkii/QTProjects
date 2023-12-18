#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H

#include <QAbstractItemModel>
#include <QQmlEngine>

typedef struct {
    QString name;
    QString phoneNumber;
} Contact;

class MyListModel : public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum ListRoles {
        NameRole = Qt::UserRole + 1,
        PhoneNumberRole = Qt::UserRole + 2,
    };

    MyListModel(QObject* parent = 0);
    ~MyListModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public slots:
    void insert(int index, const QString& name, const QString& number);
    void append(const QString& name, const QString& number);
    void append(QList<QVariantMap> items);
    void remove(int index);
    void remove(int index, int count);
    void clear();
    int count();
    QVariantMap get(int index);
    void setProperty(int index, QString property, QString value);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:  
    QList<Contact> m_items;
};


#endif // MYLISTMODEL_H
