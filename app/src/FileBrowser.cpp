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
#include <QMessageBox>

FileBrowser::FileBrowser(QWidget *parent) : QTabWidget(parent) {
    this->setMaximumWidth(parent->window()->width() / 4);
    QObject::connect(this, SIGNAL(tabCloseRequested(int)), SLOT(removeFolderCallback(int)));
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
}

void FileBrowser::addFolderCallback(const QString &sPath) {
    auto file = new QFileInfo(sPath);
    auto splitName =  sPath.split('/').last();

    if (!file->isFile() && file->isReadable() && !tabs.contains(splitName)) {
        auto layout = new QVBoxLayout();
        auto model = new FileObserver(this);
        auto widget = new QWidget(this);

        connect(model, SIGNAL(addFolderCallback(const QString&)), this, SLOT(addFolderCallback(const QString&)));

        model->setRootPath(sPath);
        layout->addWidget(model);
        widget->setLayout(layout);
        insertTab(0, widget, splitName);
        setCurrentIndex(0);
        if (tabs.empty())
            emit closeTabs();
        tabs[splitName] = model;
    }
}

void FileBrowser::removeFolderCallback(int index) {
    tabs.remove(tabText(index));
    removeTab(index);
    if (tabs.empty()) {
        emit closeTabs();
    }
}

void FileBrowser::ShowContextMenu(const QPoint &pos) {
    auto dialog = new PopupDialog(this);
    QMenu contextMenu(tr("Context menu"), this);
    QAction action1(tr("New File"), this);
    QAction action2(tr("New Folder"), this);
    QAction action3(tr("Add Project Folder"), this);
    QAction action4(tr("Remove Project Folder"), this);
    QAction action5(tr("Copy Full Path"), this);
    QAction action6(tr("Copy Project Path"), this);
    QAction action7(tr("Search File"), this);
    QAction action8(tr("Search in Folder"), this);
    QAction action9(tr("Reveal in Finder"), this);

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);
    contextMenu.addAction(&action7);
    contextMenu.addAction(&action8);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action9);

    connect(&action1, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(NewFile(const QString&)), this, SLOT(CreateFileCallback(const QString&)));
        dialog->setParams("Enter the path for the new file.", "", Type::NewFile);
    });
    connect(&action2, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(NewFolder(const QString&)), this, SLOT(CreateFolderCallback(const QString&)));
        dialog->setParams("Enter the path for the new folder.", "", Type::NewDir);
    });
    connect(&action3, &QAction::triggered, this, [this] { emit AddFileProject();});

    connect(&action4, &QAction::triggered, this, [=] { removeFolderCallback(tabBar()->tabAt(pos)); });
    connect(&action5, &QAction::triggered, this, [=] { CopyFullPathCallback(tabText(tabBar()->tabAt(pos))); });
    connect(&action6, &QAction::triggered, this, [=] { CopyFullPathCallback(tabText(tabBar()->tabAt(pos))); });
    connect(&action7, &QAction::triggered, this, [=] {
        setCurrentIndex(tabBar()->tabAt(pos));
        connect(dialog, SIGNAL(SearchFile(const QString&)), this, SLOT(SearchFileCallback(const QString&)));
        dialog->setParams("Enter the path for the search file.", "", Type::SearchFile);
    });
    connect(&action8, &QAction::triggered, this, [=] {
        connect(dialog, SIGNAL(SearchInDir(const QString&)), this, SLOT(SearchInFolderCallback(const QString&)));
        dialog->setParams("Enter smth you want to find in files.", "", Type::SearchInDir);
    });
    connect(&action9, &QAction::triggered, this, [=] {
        revealFinderCallback(tabText(tabBar()->tabAt(pos)));
    });
    contextMenu.exec(mapToGlobal(pos));
}


void FileBrowser::CreateFileCallback(const QString &sPath) {
    auto dirName = tabText(currentIndex());
    auto dirPath = static_cast<QFileSystemModel *>(tabs[dirName]->model())->filePath(tabs[dirName]->rootIndex());
    tabs[dirName]->CreateFile(dirPath + "/" + sPath);
}

void FileBrowser::CreateFolderCallback(const QString &sPath) {
    auto dirName = tabText(currentIndex());
    auto dirPath = static_cast<QFileSystemModel *>(tabs[dirName]->model())->filePath(tabs[dirName]->rootIndex());
    tabs[dirName]->CreateFolder(dirPath + "/" + sPath);
}

void FileBrowser::CopyFullPathCallback(const QString &sPath) {
    auto dirPath = static_cast<QFileSystemModel *>(tabs[sPath]->model())->filePath(tabs[sPath]->rootIndex());
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(dirPath);
}

void FileBrowser::SearchFileCallback(const QString &file) {
    auto dirName = tabText(currentIndex());
    auto dirPath = static_cast<QFileSystemModel *>(tabs[dirName]->model())->filePath(tabs[dirName]->rootIndex());
    auto model = static_cast<QFileSystemModel *>(tabs[dirName]->model());

    tabs[dirName]->setCurrentIndex(model->index(dirPath + "/" + file));
}

void FileBrowser::SearchInFolderCallback(const QString &data) {

}

void FileBrowser::revealFinderCallback(const QString &sPath) {
    auto dirPath = static_cast<QFileSystemModel *>(tabs[sPath]->model())->filePath(tabs[sPath]->rootIndex());
    auto fileInfo = QFileInfo(dirPath);

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



