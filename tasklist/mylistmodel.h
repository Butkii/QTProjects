#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H

#include <QAbstractItemModel>
#include <QQmlEngine>
#include "mylistitem.h"

typedef struct {
    QString title;
    QString description;
    bool selected;
} ListItem;

class MyListModel : public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum ListRoles {
        TitleRole = Qt::UserRole + 1,
        DescriptionRole = Qt::UserRole + 2,
        SelectedRole = Qt::UserRole + 3,
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
    void insert(int index, const QString& title, const QString& description, bool selected);
    void append(const QString& title, const QString& description, bool selected);
    void append(QList<QVariantMap> items);
    void remove(int index);
    void remove(int index, int count);
    void clear();
    int count();
    QVariantMap get(int index);
//    MyListItem get(int index);
    void setProperty(int index, QString property, QString value);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:  
    QList<MyListItem> m_items;
};


#endif // MYLISTMODEL_H
