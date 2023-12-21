#include "mylistmodel.h"
#include <QJNIObject>
#include <QJsonDocument>
#include <QGuiApplication>

static ContactListModel *instance;
QVariantMap uniqueContacts;

ContactListModel::ContactListModel(QObject* parent) : QAbstractItemModel(parent) {
    instance = this;
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    long thisAddress = (long) this;
    activity.callMethod<void>("fetchContacts", thisAddress);
}

ContactListModel::~ContactListModel() {}

extern "C" JNIEXPORT void JNICALL Java_com_contactlist_MainActivity_updateContacts(JNIEnv *env, jobject, jstring updates, jlong ptr) {
    QVariantMap res = QJsonDocument::fromJson(env->GetStringUTFChars(updates, 0)).toVariant().toMap();
    ContactListModel *test = (ContactListModel*) ptr;
    if (res.value("action").toString() == "add") {
        QList<QVariant> contactsList = res.value("contacts").toList();
        test->append(contactsList);
    } else if (res.begin().value().toString() == "delete") {
        QMap contactsList = res.value("contacts").toMap();
        test->remove(contactsList.begin().key());
    } else if (res.begin().value().toString() == "update") {
        QMap contactsList = res.value("contacts").toMap();
        qDebug() << "update list" << contactsList << contactsList.begin().key() << contactsList.begin().value().toString();
        int index = test->getByName(contactsList.begin().value().toString());
        test->setProperty(index, "phoneNumber", contactsList.begin().key());
    }
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_items.size() || index.row() < 0)
        return QVariant();

    const Contact& item = m_items[index.row()];
    switch (role) {
        case NameRole: return item.name;
        case PhoneNumberRole: return item.phoneNumber;
        case SelectedRole: return item.selected;
    }

    return QVariant();
}

Qt::ItemFlags ContactListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex ContactListModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) return QModelIndex();
    return createIndex(row, column);
}

QModelIndex ContactListModel::parent(const QModelIndex &index) const {
    return QModelIndex();
}

int ContactListModel::rowCount(const QModelIndex &parent) const {
    return m_items.size();
}

int ContactListModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

void ContactListModel::insert(int index, const QString& name, const QString& number, bool selected) {
    if (index < 0 || index > m_items.count())
        return;
    QString result;
    for (const QChar c : qAsConst(number)) {
        if (c.isDigit() || c == '.') {
            result.append(c);
        }
    }
    Contact item = {name, result, selected};

    if (!uniqueContacts.contains(number)) {
        beginInsertRows(QModelIndex(), index, index);
        m_items.insert(index, item);
        endInsertRows();
        uniqueContacts.insert(number, name);
    }
}

void ContactListModel::append(const QString& name, const QString& number) {
    int insertionPoint = count();
    for (int i = 0; i < count(); i++) {
        if(name < m_items[i].name) {
            insertionPoint = i;
            break;
        }
    }

    insert(insertionPoint, name, number);
}

void ContactListModel::append(QList<QVariant> items) {
    for (int i = 0; i < items.count(); i++) {
        QString name = items.at(i).toMap().last().toString();
        QString number = items.at(i).toMap().firstKey();
        append(name, number);
    }
}

void ContactListModel::remove(int index) {
    if (index < 0 || index > m_items.count())
        return;

    remove(index, 1);
}

void ContactListModel::remove(const QString& phoneNumber)
{
    int index = get(phoneNumber);
    if (index != -1) {
        QJniObject activity = QNativeInterface::QAndroidApplication::context();
        activity.callMethod<void>("deleteContact", "(Ljava/lang/String;)V", QJniObject::fromString(m_items.at(index).phoneNumber).object<jstring>());
        remove(index, 1);
    }
}

void ContactListModel::remove(int index, int count) {
    if (index < 0 || index > m_items.count())
        return;

    for (int i = 0; i < count; i++) {
        uniqueContacts.remove(m_items.at(index + i).phoneNumber);
    }
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

void ContactListModel::setProperty(int index, const QString &property, const QVariant &value) {
    if (index < 0 || index > m_items.count())
        return;
    qDebug("enter");
    Contact item = m_items.at(index);
    qDebug() << item.name << " " << item.phoneNumber << " " << item.selected;
    if (property == "name") {
        item.name = value.toString();
    } else if (property == "phoneNumber") {
        QString result;
        for (const QChar c : value.toString()) {
            if (c.isDigit() || c == '.') {
                result.append(c);
            }
        }
        item.phoneNumber = result;
    } else if (property == "selected") {
        item.selected = value.toBool();
    }
    qDebug() << item.name << " " << item.phoneNumber << " " << item.selected;

    remove(index);
    insert(index, item.name, item.phoneNumber, item.selected);
}

void ContactListModel::clear() {
    beginRemoveRows(QModelIndex(), 0, count());
    m_items.clear();
    endRemoveRows();
}

QVariantMap ContactListModel::get(int index)
{
    if(index < 0 || index >= m_items.count()) {
        return QVariantMap();
    }
    Contact item = m_items.at(index);
    return QVariantMap({{"name", item.name}, {"phoneNumber", item.phoneNumber}, {"selected", item.selected}});
}

int ContactListModel::get(const QString& phoneNumber)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].phoneNumber == phoneNumber) {
            return i;
        }
    }
    return -1;
}

int ContactListModel::getByName(const QString& name)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].name == name) {
            return i;
        }
    }
    return -1;
}

int ContactListModel::count() {
    return m_items.count();
}

QHash<int, QByteArray> ContactListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PhoneNumberRole] = "phoneNumber";
    roles[SelectedRole] = "selected";
    return roles;
}

void ContactListModel::add(const QString& name, const QString& number) {
    qDebug() << "called";
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    activity.callMethod<void>("addContact", "(Ljava/lang/String;Ljava/lang/String;)V", QJniObject::fromString(name).object<jstring>(), QJniObject::fromString(number).object<jstring>());
}

void ContactListModel::update(const QString& name, const QString& number) {
    qDebug() << "called update" << name << number;
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    activity.callMethod<void>("updateContact", "(Ljava/lang/String;Ljava/lang/String;)V", QJniObject::fromString(name).object<jstring>(), QJniObject::fromString(number).object<jstring>());
}
