#include "mylistmodel.h"
#include <QJNIObject>

MyListModel::MyListModel(QObject* parent) : QAbstractItemModel(parent) {
    auto list = QJniObject::callStaticObjectMethod<jstring>("com/contactlist/MainActivity", "getContactList").toString();
    QStringList contactEntries = list.split("Contact:");

    for (const QString& entry : contactEntries) {
        QStringList contactInfo = entry.split(", Phone Number:");

        if (contactInfo.size() == 2) {
            QString contactName = contactInfo[0].trimmed();
            QString phoneNumber = contactInfo[1].trimmed();

            append(contactName, phoneNumber);
        }
    }
}

MyListModel::~MyListModel() {}

QVariant MyListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_items.size() || index.row() < 0)
        return QVariant();

    const Contact& item = m_items[index.row()];
    switch (role) {
        case NameRole: return item.name;
        case PhoneNumberRole: return item.phoneNumber;
    }

    return QVariant();
}

Qt::ItemFlags MyListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex MyListModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) return QModelIndex();
    return createIndex(row, column);
}

QModelIndex MyListModel::parent(const QModelIndex &index) const {
    return QModelIndex();
}

int MyListModel::rowCount(const QModelIndex &parent) const {
    return m_items.size();
}

int MyListModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

void MyListModel::insert(int index, const QString& name, const QString& number) {
    if (index < 0 || index > m_items.count())
        return;

    Contact item = {name, number};

    beginInsertRows(QModelIndex(), index, index);
    m_items.insert(index, item);
    endInsertRows();
}

void MyListModel::append(const QString& name, const QString& number) {
    insert(count(), name, number);
}

void MyListModel::append(QList<QVariantMap> items) {
    for (int i = 0; i < items.count(); i++) {
        QString name = items.at(i)["name"].toString();
        QString number = items.at(i)["phoneNumber"].toString();

        append(name, number);
    }
}

void MyListModel::remove(int index) {
    if (index < 0 || index > m_items.count())
        return;

    remove(index, 1);
}

void MyListModel::remove(int index, int count) {
    if (index < 0 || index > m_items.count())
        return;

    if (index + count > m_items.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        m_items.remove(index, m_items.count() - index);
        endRemoveRows();
    } else {
        beginRemoveRows(QModelIndex(), index, index);
        m_items.remove(index);
        endRemoveRows();
    }
}

void MyListModel::setProperty(int index, QString property, QString value) {
    if (index < 0 || index > m_items.count())
        return;

    Contact item = m_items.at(index);
    if (property == "name") {
        item.name = value;
    } else if (property == "phoneNumber") {
        item.phoneNumber = value;
    }

    m_items.removeAt(index);
    m_items.insert(index, item);
}

void MyListModel::clear() {
    beginRemoveRows(QModelIndex(), 0, count());
    m_items.clear();
    endRemoveRows();
}

QVariantMap MyListModel::get(int index)
{
    if(index < 0 || index >= m_items.count()) {
        return QVariantMap();
    }
    Contact item = m_items.at(index);
    return QVariantMap({{"name", item.name}, {"phoneNumber", item.phoneNumber}});
}

int MyListModel::count() {
    return m_items.count();
}

QHash<int, QByteArray> MyListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PhoneNumberRole] = "phoneNumber";
    return roles;
}
