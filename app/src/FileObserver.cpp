//
// Created by Artem Shemidko on 10.11.2020.
//

#include "FileObserver.h"
#include <QFileSystemModel>
#include <QtWidgets/QMenu>
#include <QHeaderView>
#include "FileTab.h"
#include "ObserverModel.h"

FileObserver::FileObserver(QWidget *parent) : QTreeView(parent) {
    observerModel = new ObserverModel(this);
    setModel(observerModel);
    for (int i = 1; i < observerModel->columnCount(); ++i) {
        hideColumn(i);
    }
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    observerModel->setHeaderData(0,Qt::Vertical,"Project");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
}

void FileObserver::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}
void FileObserver::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}
void FileObserver::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}
void FileObserver::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls())
        return;
    auto droppedData = event->mimeData()->urls();
    if (droppedData.count() == 1) {
        if (!QDir(droppedData.at(0).toString().remove(0, 7)).exists())
            return;
        auto index = dynamic_cast<QFileSystemModel *>(model())->index(droppedData.at(0).toString().remove(0, 7));
        setRootIndex(index);
    }
}


void FileObserver::ShowContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu1"), this);
    QAction action1(tr("New File"), this);
    QAction action2(tr("New Folder"), this);
    QAction action3(tr("Rename"), this);
    QAction action4(tr("Delete"), this);
    QAction action14(tr("ppp"), this);
    auto file = dynamic_cast<QFileSystemModel *>(model())->filePath(indexAt(pos));

    contextMenu.addAction(&action1);
    connect(&action1, &QAction::triggered, this, [=] { CreateFile(file); });
    contextMenu.addAction(&action2);
    connect(&action2, &QAction::triggered, this, [=] { CreateFolder(file); });
    contextMenu.addAction(&action3);
    connect(&action3, &QAction::triggered, this, [=] { Rename(file); });
    contextMenu.addAction(&action4);
    connect(&action4, &QAction::triggered, this, [=] { Delete(file); });
    contextMenu.addAction(&action14);
    connect(&action14, &QAction::triggered, this, [=] { Delete(file); });

    contextMenu.exec(mapToGlobal(pos));
}
void FileObserver::CreateFile(const QString& sPath) {
    QFile file(sPath);
    file.open(QIODevice::ReadWrite);
    file.close();
//    auto *dialog = new TreeViewContextDialog(this);
//
//    if (path.isEmpty())
//        path = dynamic_cast<QFileSystemModel *>(model())->filePath(rootIndex());
//
//    if (dialog->exec()) {
//        if (QFileInfo(path).isDir())
//            path.append("/");
//        else
//            path.remove(path.lastIndexOf("/"), path.size()).append("/");
//        QString text = dialog->getEdit()->text();
//        QFile file(path + text);
//
//        file.open(QIODevice::ReadWrite);
//        file.close();
//    }
}
void FileObserver::CreateFolder(const QString& sPath) {
    QDir dir;
    dir.mkpath(sPath);
}
void FileObserver::Rename(QString file) {
//    TreeViewContextDialog *dialog = new TreeViewContextDialog(this);
//
//    if (dialog->exec()) {
//        QFileInfo info(file);
//        QString text = dialog->getEdit()->text();
//        QString newName = file;
//
//        newName.remove(newName.lastIndexOf("/"), newName.size()).append("/" + text);
//        if (info.isDir()) {
//            QDir dir(file);
//            dir.rename(file, newName);
//        } else {
//            QFile f(file);
//            f.rename(file, newName);
//        }
//    }
}
void FileObserver::Delete(QString file) {
//    QFileInfo info(file);
//
//    if (info.isDir()) {
//        QDir dir(file);
//        auto filelist = dir.entryInfoList();
//
//        filelist.removeFirst();
//        filelist.removeFirst();
//
//        for (const auto &editor : parent()->findChildren<TextEditor *>())
//            for (const auto &ffile : filelist)
//                if (ffile.filePath() == editor->file()->fileName())
//                    delete editor->parent();
//        dir.removeRecursively();
//    } else {
//        QFile f(file);
//
//        for (const auto &editor : parent()->findChildren<TextEditor *>())
//            if (f.fileName() == editor->file()->fileName())
//                delete editor->parent();
//        f.remove();
//    }
}

void FileObserver::setRootPath(const QString& sPath) {
    observerModel->setRootPath(sPath);
    setRootIndex(observerModel->index(sPath));
}

