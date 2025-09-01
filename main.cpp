
#include <QCoreApplication>
#include <QApplication>

#include "game_client/chess_ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    bool isServer = false;
    bool isRemote = false;
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--server") {
            qDebug() << "Starting as server...";
            isServer = true;
        } else if(QString(argv[i]) == "--clientL") {
            qDebug() << "Starting as local client...";

        } else if(QString(argv[i]) == "--clientR") {
            qDebug() << "Starting as remote client...";
            isRemote = true;
        }
        MainWindow window(isServer, isRemote);
        window.show();
        return app.exec();
    }
    return 0;
}
