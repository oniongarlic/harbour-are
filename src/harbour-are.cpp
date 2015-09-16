#include <QtQuick>
#include <QDebug>
#include <sailfishapp.h>

#include "MapWidget.h"
#include "positionsource.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    qRegisterMetaType<DatabaseLoadedResponse>();

    qmlRegisterType<MapWidget>("net.sf.libosmscout.map", 1, 0, "Map");
    qmlRegisterType<Location>("net.sf.libosmscout.map", 1, 0, "Location");
    qmlRegisterType<LocationListModel>("net.sf.libosmscout.map", 1, 0, "LocationListModel");

    qmlRegisterType<PositionSource>("org.tal.gps", 1, 0, "PositionSource");

    app->setApplicationName("harbour-are");

    QThread thread;

    if (!DBThread::InitializeInstance()) {
        qDebug() << "Cannot initialize DBThread";
    }

    DBThread* dbThread=DBThread::GetInstance();

    dbThread->connect(&thread, SIGNAL(started()), SLOT(Initialize()));
    dbThread->connect(&thread, SIGNAL(finished()), SLOT(Finalize()));
    dbThread->moveToThread(&thread);    

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setSource(SailfishApp::pathTo("qml/harbour-are.qml"));
    view->showFullScreen();
    thread.start();

    app->exec();

    thread.quit();
    thread.wait();

    DBThread::FreeInstance();

    return 0;
}

