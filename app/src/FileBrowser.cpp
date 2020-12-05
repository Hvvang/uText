//
// Created by Artem Shemidko on 05.12.2020.
//
#include "FileBrowser.h"
#include "FileObserver.h"
#include "PopupDialog.h"
#include "mainwindow.h"
#include <QFileInfo>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>


FileBrowser::FileBrowser(QWidget *parent) : QTabWidget(parent) {
    this->setMaximumWidth(parent->window()->width() / 4);
    QObject::connect(this, SIGNAL(tabCloseRequested(int)), SLOT(removeFolder(int)));
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
}

void FileBrowser::addFolder(const QString &sPath) {
    auto file = new QFileInfo(sPath);
    if (!file->isFile() && file->isReadable() && !tabs.contains(sPath)) {
        auto layout = new QVBoxLayout();
        auto model = new FileObserver(this);
        auto widget = new QWidget(this);

        model->setRootPath(sPath);
        layout->addWidget(model);
        widget->setLayout(layout);
        insertTab(0, widget, sPath);
        if (tabs.empty())
                emit closeTabs();
        tabs[sPath] = model;
    }
}

void FileBrowser::removeFolder(int index) {
    tabs.remove(tabText(index));
    removeTab(index);
    if (tabs.empty()) {
        emit closeTabs();
    }
}

void FileBrowser::ShowContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);
    QAction action1(tr("New File"), this);
    QAction action2(tr("New Folder"), this);
    QAction action3(tr("Rename"), this);
    QAction action4(tr("Delete"), this);
    QAction action5(tr("Add Project Folder"), this);
    QAction action6(tr("Remove Project Folder"), this);
    QAction action7(tr("Copy Full Path"), this);
    QAction action8(tr("Search File"), this);
    QAction action9(tr("Search in Folder"), this);

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action7);
    contextMenu.addAction(&action8);
    contextMenu.addAction(&action9);


    connect(&action1, &QAction::triggered, this, [this] {
        new PopupDialog("Enter the path for the new file.", Type::NewFile, this);
    });
    connect(&action2, &QAction::triggered, this, [this] {
        new PopupDialog("Enter the path for the new folder.", Type::NewDir, this);
    });
//    connect(&action2, &QAction::triggered, this, [=] { CreateFile(file); });
//    connect(&action3, &QAction::triggered, this, [=] { CreateFile(file); });
//    connect(&action4, &QAction::triggered, this, [=] { CreateFile(file); });
    contextMenu.exec(mapToGlobal(pos));

//    contextMenu.addAction(&action5);

//    auto file = dynamic_cast<QFileSystemModel *>(model())->filePath(indexAt(pos));
}

void FileBrowser::CreateFile(const QString &sPath) {
    auto dirPath = tabText(currentIndex());
    tabs[dirPath]->CreateFile(dirPath + "/" + sPath);
}

void FileBrowser::CreateFolder(const QString &sPath) {
    auto dirPath = tabText(currentIndex());
    tabs[dirPath]->CreateFolder(dirPath + "/" + sPath);
}
