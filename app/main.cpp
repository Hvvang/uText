#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    if (!QDir(argv[1]).exists() && argv[1]) {
//        ErrorDialog error("Invalid folder path");
//    } else {
        MainWindow w((QString(argv[1])));
        w.show();
        return a.exec();
//    }
}
