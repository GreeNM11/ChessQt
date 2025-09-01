
#include <QCoreApplication>
#include <QApplication>

#include "game_client/chess_ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qDebug() << "Chess.exe opened";
    bool isServer = false;
    bool isRemote = true;
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--server") {
            qDebug() << "Starting as server...";
            isServer = true;
        } else if(QString(argv[i]) == "--clientL") {
            qDebug() << "Starting as local client...";
            isRemote = false;

        } else if(QString(argv[i]) == "--clientR") {
            qDebug() << "Starting as remote client...";
        }
    }
    if (argc == 0){
        qDebug() << "Default ~ Starting as remote client...";
    }
    MainWindow window(isServer, isRemote);
    window.show();
    return app.exec();
}
