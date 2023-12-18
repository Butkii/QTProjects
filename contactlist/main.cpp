#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mylistmodel.h"

#include <QDebug>
#include <QJNIObject>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<MyListModel>("MyListModel", 1, 0, "MyListModel");

    const QUrl url(u"qrc:/contactlist/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
