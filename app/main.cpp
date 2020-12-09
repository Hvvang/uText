#include "mainwindow.h"
#include <QApplication>
#include <QErrorMessage>
#include "WindowManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    if ((!QDir(argv[1]).exists() || !QFile(argv[1]).exists()) && argv[1]) {
        QErrorMessage msg;
        msg.showMessage("Invalid path to file or folder!");
        msg.exec();
        return 1;
    } else {
        WindowManager manager;
//        w.show();
//        MainWindow w((QString(argv[1])));
//        w.show();
        emit manager.addWindow( QString(argv[1]));
        return a.exec();
    }
}
