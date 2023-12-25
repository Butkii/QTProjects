#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "contactlistmodel.h"

#include <QDebug>
#include <QJNIObject>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<ContactListModel>("ContactListModel", 1, 0, "ContactListModel");

    const QUrl url(u"qrc:/contactlist/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
