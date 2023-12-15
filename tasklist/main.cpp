#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mylistmodel.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

//    MyListModel listmodel;
//    listmodel.addItem("Task1", "desc", false);
//    listmodel.addItem("Task2", "desc", true);
//    engine.rootContext()->setContextProperty("listmodel", &listmodel);


    qmlRegisterType<MyListModel>("MyListModel", 1, 0, "MyListModel");
    qRegisterMetaType<MyListItem>("MyListItem");

    const QUrl url(u"qrc:/tasklist/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
