#include "mainwindow.h"
#include <QApplication>
#include <QErrorMessage>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    if ((!QDir(argv[1]).exists() || !QFile(argv[1]).exists()) && argv[1]) {
        QErrorMessage msg;
        msg.showMessage("Invalid path to file or folder!");
        msg.exec();
        return 1;
    } else {
        MainWindow w((QString(argv[1])));
        w.show();
        return a.exec();
    }
}
