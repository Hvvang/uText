//
// Created by Artem Shemidko on 05.12.2020.
//
#include "FileBrowser.h"
#include "FileObserver.h"
#include "mainwindow.h"
#include <QFileInfo>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

FileBrowser::FileBrowser(QWidget *parent) : QTabWidget(parent) {
    this->setMaximumWidth(parent->window()->width() / 4);
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(removeFolderCallback(int)));
}

void FileBrowser::addFolderCallback(const QString &sPath) {
    auto file = new QFileInfo(sPath);
    auto splitName = sPath.split('/').last();

    if (!file->isFile() && file->isReadable() && !tabs.contains(splitName)) {
        auto layout = new QVBoxLayout();
        auto model = new FileObserver(this);
        auto widget = new QWidget(this);

        connect(model, &FileObserver::oneClick, this, &FileBrowser::oneClickCallback);
        connect(model, &FileObserver::doubleClick, this, &FileBrowser::doubleClickCallback);
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

void FileBrowser::CreateFileCallback(const QString &sPath) {
    auto dirName = tabText(currentIndex());
    auto dirPath = static_cast<QFileSystemModel *>(tabs[dirName]->model())->filePath(tabs[dirName]->rootIndex());
    tabs[dirName]->CreateFile(dirPath + "/" + sPath);
    emit doubleClick(dirPath + "/" + sPath);
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
    emit doubleClick(dirPath + "/" + file);
}

void FileBrowser::SearchInFolderCallback(const QString &data) {

}

void FileBrowser::revealFinderCallback(const QString &sPath) {
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

void FileBrowser::oneClickCallback(const QString &sPath) {
    emit oneClick(sPath);
}

void FileBrowser::doubleClickCallback(const QString &sPath) {
    emit doubleClick(sPath);
}

const QMap<QString, FileObserver *> &FileBrowser::Tabs() const {
    return tabs;
}
