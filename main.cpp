#include "mainwindow.h"
#include "server.h"
#include "client.h"

#include <QCoreApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--server") {
            qDebug() << "Starting as server...";
            Server s;
            return app.exec();
        } else if(QString(argv[i]) == "--client") {
            qDebug() << "Starting as client...";
            MainWindow w;
            w.show();
            return app.exec();
        }
    }
    return 0;
}
