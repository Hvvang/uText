#include "ui_mainwindow.h"
#include "mainwindow.h"


MainWindow::MainWindow(QString sPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

