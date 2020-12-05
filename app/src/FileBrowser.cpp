//
// Created by Artem Shemidko on 05.12.2020.
//
#include "FileBrowser.h"
#include "FileObserver.h"
#include <QFileInfo>
#include <QVBoxLayout>

FileBrowser::FileBrowser(QWidget *parent) : QTabWidget(parent) {
    QObject::connect(this, SIGNAL(tabCloseRequested(int)), SLOT(removeFolder(int)));
}

void FileBrowser::addFolder(const QString &sPath) {
    auto file = new QFileInfo(sPath);
    if (!file->isFile() && file->isReadable()) {
        auto layout = new QVBoxLayout();
        auto model = new FileObserver(this);
        auto widget = new QWidget(this);

        model->setRootPath(sPath);
        layout->addWidget(model);
        widget->setLayout(layout);
        insertTab(0, widget, sPath);
    }
}

void FileBrowser::removeFolder(int index) {
    removeTab(index);
}