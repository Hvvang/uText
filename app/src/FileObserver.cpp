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
#include <QFileDialog>
#include <QMessageBox>

FileObserver::FileObserver(QWidget *parent) : QTreeView(parent) {
    observerModel = new ObserverModel(this);
    setModel(observerModel);
    for (int i = 1; i < observerModel->columnCount(); ++i) {
        hideColumn(i);
    }
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    setDragEnabled(true);
    setDragDropMode(DragDropMode::DropOnly);
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
    if (!event->mimeData()->hasUrls()) {
        return;
    }
    auto droppedData = event->mimeData()->urls();
    if (droppedData.count() == 1) {
        auto dir = QDir(droppedData.at(0).toString().remove(0, 7));
        if (!dir.exists())
            return;
        emit addFolderCallback(dir.canonicalPath());
    }
}

void FileObserver::setRootPath(const QString& sPath) {
    observerModel->setRootPath(sPath);
    setRootIndex(observerModel->index(sPath));
}

void FileObserver::ShowContextMenu(const QPoint &pos) {
    auto dialog = new PopupDialog(this);
    auto file = static_cast<QFileSystemModel *>(model())->filePath(indexAt(pos));
    QFileInfo info(file);
    auto dirPath = static_cast<QFileSystemModel *>(model())->filePath(rootIndex());
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
        dialog->setParams("Enter smth you want to find in files.", "", Type::SearchInDir);
    });
    connect(&action2, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(NewFile(const QString&)), this, SLOT(PreCreateFileCallback(const QString&)));
        auto temp = entry;
        auto res = (info.isDir()) ? temp : temp.remove(temp.lastIndexOf('/'), temp.size());
        dialog->setParams("Enter the path for the new file.", res  + '/', Type::NewFile);
    });
    connect(&action3, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(NewFolder(const QString&)), this, SLOT(PreCreateDirCallback(const QString&)));
        auto temp = entry;
        auto res = (info.isDir()) ? temp : temp.remove(temp.lastIndexOf('/'), temp.size());
        dialog->setParams("Enter the path for the new file.", res  + '/', Type::NewDir);
    });
    connect(&action4, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(Rename(const QString&)), this, SLOT(Rename(const QString&)));
        dialog->setParams("Enter the new path for the " + (info.isFile()) ? "file." : "folder.", entry, Type::Rename);
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
    auto dirPath = static_cast<QFileSystemModel *>(model())->filePath(rootIndex());
    CreateFile(dirPath + sPath);
}

void FileObserver::PreCreateDirCallback(const QString &sPath) {
    auto dirPath = static_cast<QFileSystemModel *>(model())->filePath(rootIndex());
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
    auto path = static_cast<QFileSystemModel *>(model())->filePath(currentIndex());

    auto res = path;
    QFileInfo info(path);
    res.remove(res.lastIndexOf("/"), res.size()).append("/" + newName);
    if (info.isDir()) {
        QDir dir(path);
        dir.rename(path, res);
    } else {
        QFile file(path);
        file.rename(path, res);
    }
}

void copyPath(QString src, QString dst) {
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
}

void FileObserver::CutItem(const QString &file) {
// paste files/folders to clipboard doesn`t support on mac
}

void FileObserver::PasteItem(const QString &file) {
// paste files/folders to clipboard doesn`t support on mac
}

void FileObserver::openCallback() {
    QFileDialog dialog;
    dialog.setOptions(QFileDialog::DontResolveSymlinks);
    dialog.setNameFilter(tr("Open..."));
    if (dialog.exec() == QDialog::Accepted) {
        QString dirName = dialog.selectedFiles().front();
        if (QDir(dirName).isReadable()) {
            emit addFolderCallback(dirName);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Not enough permission!"), QMessageBox::Ok);
        }
    }
}

void FileObserver::RevealInFinder(const QString &sPath) {
    auto fileInfo = QFileInfo(sPath);

#ifdef WIN
    const FileName explorer = Environment::systemEnvironment().searchInPath(QLatin1String("explorer.exe"));
        if (explorer.isEmpty()) {
            QMessageBox::warning(parent,
                                 QApplication::translate("Core::Internal",
                                                         "Launching Windows Explorer Failed"),
                                 QApplication::translate("Core::Internal",
                                                         "Could not find explorer.exe in path to launch Windows Explorer."));
            return;
        }
        QStringList param;
        if (!fileInfo.isDir())
            param += QLatin1String("/select,");
        param += QDir::toNativeSeparators(fileInfo.canonicalFilePath());
        QProcess::startDetached(explorer.toString(), param);
#else
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                       .arg(fileInfo.canonicalFilePath());
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
#endif
}

void FileObserver::CopyPath(const QString &path) {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(path);
}


