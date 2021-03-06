//
// Created by Artem Shemidko on 10.11.2020.
//

#include "FileObserver.h"
#include <QFileSystemModel>
#include <QtWidgets/QMenu>
#include <QHeaderView>
#include "FileTab.h"
#include "ObserverModel.h"
#include "PopupDialog.h"
#include <QtGui>
#include <QMessageBox>

FileObserver::FileObserver(QWidget *parent) : QTreeView(parent) {
    observerModel = new ObserverModel(this);
    setModel(observerModel);
    for (int i = 1; i < observerModel->columnCount(); ++i) {
        hideColumn(i);
    }
    setMinimumWidth(0);
    dialog = new PopupDialog(this);
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    observerModel->setHeaderData(0, Qt::Vertical,"Project");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
}

FileObserver::~FileObserver() {
    if (observerModel) delete observerModel;
    if (dialog) delete dialog;
}

void FileObserver::setRootPath(const QString& sPath) {
    observerModel->setRootPath(sPath);
    setRootIndex(observerModel->index(sPath));
}

void FileObserver::ShowContextMenu(const QPoint &pos) {
    auto file = dynamic_cast<QFileSystemModel *>(model())->filePath(indexAt(pos));
    QFileInfo info(file);
    auto dirPath = dynamic_cast<QFileSystemModel *>(model())->filePath(rootIndex());
    auto entry = file;
    entry = entry.remove(0, dirPath.size());

    QMenu contextMenu(tr("Context menu1"), this);
    QAction action1(tr("Search in Folder"), this);
    QAction action2(tr("New File"), this);
    QAction action3(tr("New Folder"), this);
    QAction action4(tr("Rename"), this);
    QAction action5(tr("Duplicate"), this);
    QAction action6(tr("Delete"), this);
    QAction action7(tr("Copy"), this);
    QAction action8(tr("Cut"), this);
    QAction action9(tr("Paste"), this);
    QAction action10(tr("Add Project Folder"), this);
    QAction action11(tr("Copy Full Path"), this);
    QAction action12(tr("Copy Project Path"), this);
    QAction action13(tr("Reveal in Finder"), this);

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(&action3);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action4);
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);
    contextMenu.addAction(&action7);
    contextMenu.addAction(&action8);
    contextMenu.addAction(&action9);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action10);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action11);
    contextMenu.addAction(&action12);
    contextMenu.addAction(&action13);



    connect(&action1, &QAction::triggered, this, [=] {
//        connect(dialog, SIGNAL(NewFile(const QString&)), this, SLOT(PreCreateFileCallback(const QString&)));
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter smth you want to find in files.", "", Type::SearchInDir);
    });
    connect(&action2, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(NewFile(const QString&)), this, SLOT(PreCreateFileCallback(const QString&)));
        auto temp = entry;
        auto res = (info.isDir()) ? temp : temp.remove(temp.lastIndexOf('/'), temp.size());
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter the path for the new file.", res  + '/', Type::NewFile);
    });
    connect(&action3, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(NewFolder(const QString&)), this, SLOT(PreCreateDirCallback(const QString&)));
        auto temp = entry;
        auto res = (info.isDir()) ? temp : temp.remove(temp.lastIndexOf('/'), temp.size());
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter the path for the new file.", res  + '/', Type::NewDir);
    });
    connect(&action4, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(Rename(const QString&)), this, SLOT(Rename(const QString&)));
        QString type = (info.isFile()) ? "file." : "folder.";
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter the new path for the " + type, entry, Type::Rename);
    });
    connect(&action5, &QAction::triggered, this, [=] {
        DuplicateItem(file);
    });
    connect(&action6, &QAction::triggered, this, [=] {
        DeleteItem(file);
    });
    connect(&action7, &QAction::triggered, this, [=] {
        CopyItem(file);
    });
    connect(&action8, &QAction::triggered, this, [=] {
        CutItem(file);
    });
    connect(&action9, &QAction::triggered, this, [=] {
        PasteItem(file);
    });
    connect(&action10, &QAction::triggered, this, [=] {
        openCallback();
    });
    connect(&action11, &QAction::triggered, this, [=] {
        CopyPath(file);
    });
    connect(&action12, &QAction::triggered, this, [=] {
        CopyPath(dirPath);
    });
    connect(&action13, &QAction::triggered, this, [=] {
        RevealInFinder(file);
    });

    contextMenu.exec(mapToGlobal(pos));
}

void FileObserver::PreCreateFileCallback(const QString &sPath) {
    auto dirPath = dynamic_cast<QFileSystemModel *>(model())->filePath(rootIndex());
    CreateFile(dirPath + sPath);
}

void FileObserver::PreCreateDirCallback(const QString &sPath) {
    auto dirPath = dynamic_cast<QFileSystemModel *>(model())->filePath(rootIndex());
    CreateFolder(dirPath + sPath);
}

void FileObserver::CreateFile(const QString& sPath) {
    QFile file(sPath);
    file.open(QIODevice::ReadWrite);
    file.close();
}

void FileObserver::CreateFolder(const QString& sPath) {
    QDir dir;
    dir.mkpath(sPath);
}

void FileObserver::Rename(const QString& newName) {
    auto path = dynamic_cast<QFileSystemModel *>(model())->filePath(currentIndex());

    auto res = dynamic_cast<QFileSystemModel *>(model())->filePath(rootIndex());
    QFileInfo info(path);
    res.append(newName);

    if (path != res) {
        if (info.isDir()) {
            QDir dir(path);
            dir.rename(path, res);
            emit dynamic_cast<FileBrowser *>(parentWidget()->parentWidget()->parentWidget())->FileRenameByDirRename(path, res);

        } else {
            QFile file(path);
            file.rename(path, res);
            emit dynamic_cast<FileBrowser *>(parentWidget()->parentWidget()->parentWidget())->FileRename(path, res);
        }
    }
}

void copyPath(const QString &src, const QString &dst) {
    QDir dir(src);
    if (! dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src+ QDir::separator() + d, dst_path);
    }
    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

void FileObserver::DuplicateItem(const QString &fileName) {
    QFileInfo info(fileName);

    QString newDirName;
    int i = -1;
    do {
        newDirName = fileName + "_" + QString::number(++i);
        if (i == 127)
            throw std::runtime_error("Couldn't duplicate item.");
    } while (QFileInfo::exists(newDirName));
    if (info.isDir()) {
        CreateFolder(newDirName);
        copyPath(fileName, newDirName);
    }
    else {
        QFile::copy(fileName, newDirName);
    }
}

void FileObserver::DeleteItem(const QString &file) {
    QFileInfo info(file);

    if (info.isDir()) {
        QDir dir(file);
        auto filelist = dir.entryInfoList();

        filelist.removeFirst();
        filelist.removeFirst();

//        for (const auto &editor : parent()->findChildren<TextEditor *>())
//            for (const auto &ffile : filelist)
//                if (ffile.filePath() == editor->file()->fileName())
//                    delete editor->parent();
        dir.removeRecursively();
    } else {
        QFile f(file);
//        for (const auto &editor : parent()->findChildren<TextEditor *>())
//            if (f.fileName() == editor->file()->fileName())
//                delete editor->parent();
        f.remove();
    }
}

void FileObserver::CopyItem(const QString &file) {
// copy files/folders to clipboard doesn`t support on mac
    qDebug() << "[WARNING] - COPY FILE - " << file << " - copy files/folders to clipboard doesn`t support on mac";
}

void FileObserver::CutItem(const QString &file) {
// copy/paste files/folders to clipboard doesn`t support on mac
    qDebug() << "[WARNING] - CUT FILE - " << file << " - copy/paste files/folders to clipboard doesn`t support on mac";

}

void FileObserver::PasteItem(const QString &file) {
// paste files/folders to clipboard doesn`t support on mac
    qDebug() << "[WARNING] - PASTE FILE - " << file << " - paste files/folders to clipboard doesn`t support on mac";

}

void FileObserver::openCallback() {
    emit dynamic_cast<FileBrowser *>(parentWidget()->parentWidget()->parentWidget())->AddFileProject();
}

void FileObserver::RevealInFinder(const QString &sPath) {
    dynamic_cast<FileBrowser *>(parentWidget()->parentWidget()->parentWidget())->revealFinderCallback(sPath);
}

void FileObserver::CopyPath(const QString &path) {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(path);
}

void FileObserver::mouseDoubleClickEvent(QMouseEvent *event) {
    QTreeView::mouseDoubleClickEvent(event);
    auto *file = new QFileInfo(dynamic_cast<QFileSystemModel *>(model())->filePath(indexAt(event->pos())));
    if (file->isFile()) {
        emit doubleClick(file->filePath());
    }
    delete file;
}

void FileObserver::mouseReleaseEvent(QMouseEvent *event) {
    QTreeView::mouseReleaseEvent(event);
    auto *file = new QFileInfo(dynamic_cast<QFileSystemModel *>(model())->filePath(indexAt(event->pos())));
    if (file->isFile()) {
        emit oneClick(file->filePath());
    }
    delete file;
}
