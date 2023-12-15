#include "mylistitem.h"
#include "QtCore/qdebug.h"

MyListItem::MyListItem(const MyListItem& other)
    : m_title(other.m_title), m_description(other.m_description), m_selected(other.m_selected) { }

const QString& MyListItem::title() const { return m_title; }
void MyListItem::setTitle(const QString& title) {
    m_title = title;
    emit titleChanged(m_title);
}

const QString& MyListItem::description() const { return m_description; }
void MyListItem::setDescription(const QString& description) {
    m_description = description;
    emit descriptionChanged(m_description);
}

bool MyListItem::selected() const { return m_selected; }
void MyListItem::setSelected(bool selected) {
    m_selected = selected;
    qDebug()<<"Selected "<< selected;
    emit selectChanged(m_selected);
}

MyListItem& MyListItem::operator=(const MyListItem& other)
{
    if (this != &other) {
        m_title = other.m_title;
        m_description = other.m_description;
        m_selected = other.m_selected;
    }
    return *this;
}
