#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "FileObserver.h"
//#include <QFileSystemModel>


MainWindow::MainWindow(QString sPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->treeView->setRootPath(sPath);
    ui->textEdit->setHidden(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

