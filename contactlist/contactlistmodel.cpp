#include "contactlistmodel.h"
#include <QJNIObject>
#include <QJsonDocument>
#include <QGuiApplication>

static ContactListModel *instance;
QVariantMap uniqueContacts;

QString formatPhoneNumber(const QString& phoneNumber) {
    QString result;
    for (const QChar c : phoneNumber) {
        if (c.isDigit() || c == '.') {
            result.append(c);
        }
    }

    return result;
}

ContactListModel::ContactListModel(QObject* parent) : QAbstractItemModel(parent) {
    instance = this;
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    activity.callMethod<void>("fetchContacts", (long) this);
}

ContactListModel::~ContactListModel() {}

extern "C" JNIEXPORT void JNICALL Java_com_contactlist_MainActivity_updateContacts(JNIEnv *env, jobject, jstring updates, jlong ptr) {
    QVariantMap res = QJsonDocument::fromJson(env->GetStringUTFChars(updates, 0)).toVariant().toMap();
    ContactListModel *test = (ContactListModel*) ptr;
    QMetaObject::invokeMethod(test, "updateContactsOnMainThread",
                              Qt::QueuedConnection,
                              Q_ARG(QVariantMap, res),
                              Q_ARG(ContactListModel*, test));
}

void ContactListModel::updateContactsOnMainThread(const QVariantMap &update, ContactListModel *test) {
    qDebug() << QThread::currentThread();
    QString action = update.value("action").toString();

    if (action == "add") {
        append(update.value("contacts").toList());
    } else if (action == "delete") {
        remove(update.value("contacts").toMap().begin().key());
    } else if (action == "update") {
        QMap contactsList = update.value("contacts").toMap();
        int index = getByName(contactsList.begin().value().toString());
        setProperty(index, "phoneNumber", contactsList.begin().key());
        index = test->get(contactsList.begin().key());
        test->setProperty(index, "name", contactsList.begin().value().toString());
    }
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_items.size() || index.row() < 0)
        return QVariant();

    const QVariantMap& item = m_items[index.row()];
    switch (role) {
        case NameRole: return item.value("name");
        case PhoneNumberRole: return item.value("phoneNumber");
        case SelectedRole: return item.value("selected");
        case SectionRole: return item.value("name").toString().toUpper()[0];
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

    QVariantMap item;
    item["name"] = name;
    item["phoneNumber"] = formatPhoneNumber(number);
    item["selected"] = selected;

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
        if(name.toUpper().at(0) < m_items[i]["name"].toString().toUpper().at(0)) {
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
        activity.callMethod<void>("deleteContact", "(Ljava/lang/String;)V",QJniObject::fromString(m_items.at(index)["phoneNumber"].toString()).object<jstring>());
        remove(index, 1);
    }
}

void ContactListModel::remove(int index, int count) {
    if (index < 0 || index > m_items.count())
        return;

    for (int i = 0; i < count; i++) {
        uniqueContacts.remove(m_items.at(index + i)["phoneNumber"].toString());
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
    QVariantMap item = m_items.at(index);
    if (property == "name") {
        item["name"] = value.toString();
    } else if (property == "phoneNumber") {
        item["phoneNumber"] = formatPhoneNumber(value.toString());
    } else if (property == "selected") {
        item["selected"] = value.toBool();
    }

    remove(index);
    insert(index, item["name"].toString(), item["phoneNumber"].toString(), item["selected"].toBool());
}

void ContactListModel::setProperty(const QString &property, const QVariant &value) {
    for (int i = 0; i < count(); i++) {
        if (get(i).value(property) != value.toString()) {
            setProperty(i, property, value);
        }
    }
}

void ContactListModel::clear() {
    beginRemoveRows(QModelIndex(), 0, count());
    m_items.clear();
    endRemoveRows();
}

QVariantMap ContactListModel::get(int index)
{
    if(index < 0 || index >= m_items.count())
        return QVariantMap();
    return m_items.at(index);
}

int ContactListModel::get(const QString& phoneNumber)
{
    QString formatted = formatPhoneNumber(phoneNumber);
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i]["phoneNumber"].toString() == formatted)
            return i;
    }

    return -1;
}

int ContactListModel::getByName(const QString& name)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i]["name"].toString().compare(name) == 0)
            return i;
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
    roles[SectionRole] = "section";
    return roles;
}

void ContactListModel::add(const QString& name, const QString& number) {
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    activity.callMethod<void>("addContact", "(Ljava/lang/String;Ljava/lang/String;)V", QJniObject::fromString(name).object<jstring>(), QJniObject::fromString(number).object<jstring>());
}

void ContactListModel::update(const QString& name, const QString& number) {
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    if (uniqueContacts.contains(number))
        activity.callMethod<void>("updateContactName", "(Ljava/lang/String;Ljava/lang/String;)V", QJniObject::fromString(number).object<jstring>(), QJniObject::fromString(name).object<jstring>());
    else
        activity.callMethod<void>("updateContactNumber", "(Ljava/lang/String;Ljava/lang/String;)V", QJniObject::fromString(name).object<jstring>(), QJniObject::fromString(number).object<jstring>());
}

