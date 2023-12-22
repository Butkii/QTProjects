#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H

#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QTimer>

class ContactListModel : public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum ListRoles {
        NameRole,
        PhoneNumberRole,
        SelectedRole,
        SectionRole
    };

    ContactListModel(QObject* parent = 0);
    ~ContactListModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public slots:
    void insert(int index, const QString& name, const QString& number, bool selected = false);
    void append(const QString& name, const QString& number);
    void append(QList<QVariant> items);
    void remove(const QString& phoneNumber);
    void remove(int index);
    void remove(int index, int count);
    void clear();
    int count();
    QVariantMap get(int index);
    int get(const QString& phoneNumber);
    int getByName(const QString& name);
    void setProperty(int index, const QString& property, const QVariant &value);
    void setProperty(const QString& property, const QVariant &value);
    void add(const QString& name, const QString& number);
    void update(const QString &name, const QString &number);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:  
    QList<QVariantMap> m_items;
};

#endif // MYLISTMODEL_H
