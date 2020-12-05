#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "FileObserver.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QString sPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->textEdit->setHidden(true);
    menuConnector();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::menuConnector() {
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newCallback);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openCallback);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveCallback);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAsCallback);
    connect(ui->actionSave_all, &QAction::triggered, this, &MainWindow::saveAllCallback);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeCallback);
}

void MainWindow::openCallback() {
    QFileDialog dialog;
    dialog.setOptions(QFileDialog::DontResolveSymlinks);
    dialog.setNameFilter(tr("Open..."));
    if (dialog.exec() == QDialog::Accepted) {
        QString dirName = dialog.selectedFiles().front();
        if (QDir(dirName).isReadable()) {
            ui->BrowserView->addFolder(dirName);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Not enough permission!"), QMessageBox::Ok);
        }
    }
}

void MainWindow::newCallback() {
//  create new tab in panel
}

void MainWindow::saveCallback() {

}

void MainWindow::saveAsCallback() {

}

void MainWindow::saveAllCallback() {

}

void MainWindow::closeCallback() {

}
