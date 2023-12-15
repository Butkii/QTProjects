#include "mylistmodel.h"

MyListModel::MyListModel(QObject* parent) : QAbstractItemModel(parent) {}

MyListModel::~MyListModel() {}

QVariant MyListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_items.size() || index.row() < 0)
        return QVariant();

    const MyListItem& item = m_items[index.row()];
    switch (role) {
        case TitleRole: return item.title();
        case DescriptionRole: return item.description();
        case SelectedRole: return item.selected();
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

void MyListModel::insert(int index, const QString& title, const QString& description, bool selected) {
    if (index < 0 || index > m_items.count())
        return;

    MyListItem item = {title, description, selected};

    beginInsertRows(QModelIndex(), index, index);
    m_items.insert(index, item);
    endInsertRows();
}

void MyListModel::append(const QString& title, const QString& description, bool selected) {
    insert(count(), title, description, selected);
}

void MyListModel::append(QList<QVariantMap> items) {
    for (int i = 0; i < items.count(); i++) {
        QString title = items.at(i)["title"].toString();
        QString description = items.at(i)["description"].toString();
        bool selected = items.at(i)["selected"].toBool();

        append(title, description, selected);
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

    MyListItem item = m_items.at(index);
    if (property == "title") {
        item.setTitle(value);
    } else if (property == "description") {
        item.setDescription(value);
    } else if (property == "selected") {
        item.setSelected(value == "true");
    }

    m_items.removeAt(index);
    m_items.insert(index, item);
}

void MyListModel::clear() {
    beginRemoveRows(QModelIndex(), 0, count());
    m_items.clear();
    endRemoveRows();
}

//MyListItem MyListModel::get(int index)
//{
//    if(index < 0 || index >= m_items.count()) {
//        return MyListItem();
//    }
//    MyListItem item = m_items.at(index);
//    return item;
//}


QVariantMap MyListModel::get(int index)
{
    if(index < 0 || index >= m_items.count()) {
        return QVariantMap();
    }
    MyListItem item = m_items.at(index);
    return QVariantMap({{"title", item.title()}, {"description", item.description()}, {"selected", item.selected()}});
}

int MyListModel::count() {
    return m_items.count();
}

QHash<int, QByteArray> MyListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[DescriptionRole] = "description";
    roles[SelectedRole] = "selected";
    return roles;
}
