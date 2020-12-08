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
    this->setStyleSheet(
            "QTabWidget::pane {\n"
            "    border: 1px solid black;\n"
            "    background: white;\n"
            "}\n"
            "\n"
            "QTabWidget::tab-bar:top {\n"
            "    top: 1px;\n"
            "}\n"
            "\n"
            "QTabWidget::tab-bar:bottom {\n"
            "    bottom: 1px;\n"
            "}\n"
            "\n"
            "QTabWidget::tab-bar:left {\n"
            "    right: 1px;\n"
            "}\n"
            "\n"
            "QTabWidget::tab-bar:right {\n"
            "    left: 1px;\n"
            "}\n"
            "\n"
            "QTabBar::tab {\n"
            "    border: 1px solid black;\n"
            "}\n"
            "\n"
            "QTabBar::tab:selected {\n"
            "    background: white;\n"
            "}\n"
            "\n"
            "QTabBar::tab:!selected {\n"
            "    background: silver;\n"
            "}\n"
            "\n"
            "QTabBar::tab:!selected:hover {\n"
            "    background: #999;\n"
            "}\n"
            "\n"
            "QTabBar::tab:top:!selected {\n"
            "    margin-top: 3px;\n"
            "}\n"
            "\n"
            "QTabBar::tab:bottom:!selected {\n"
            "    margin-bottom: 3px;\n"
            "}\n"
            "\n"
            "QTabBar::tab:top, QTabBar::tab:bottom {\n"
            "    min-width: 8ex;\n"
            "    margin-right: -1px;\n"
            "    padding: 5px 10px 5px 10px;\n"
            "}\n"
            "\n"
            "QTabBar::tab:top:selected {\n"
            "    border-bottom-color: none;\n"
            "}\n"
            "\n"
            "QTabBar::tab:bottom:selected {\n"
            "    border-top-color: none;\n"
            "}\n"
            "\n"
            "QTabBar::tab:top:last, QTabBar::tab:bottom:last,\n"
            "QTabBar::tab:top:only-one, QTabBar::tab:bottom:only-one {\n"
            "    margin-right: 0;\n"
            "}\n"
            "\n"
            "QTabBar::tab:left:!selected {\n"
            "    margin-right: 3px;\n"
            "}\n"
            "\n"
            "QTabBar::tab:right:!selected {\n"
            "    margin-left: 3px;\n"
            "}\n"
            "\n"
            "QTabBar::tab:left, QTabBar::tab:right {\n"
            "    min-height: 8ex;\n"
            "    margin-bottom: -1px;\n"
            "    padding: 10px 5px 10px 5px;\n"
            "}\n"
            "\n"
            "QTabBar::tab:left:selected {\n"
            "    border-left-color: none;\n"
            "}\n"
            "\n"
            "QTabBar::tab:right:selected {\n"
            "    border-right-color: none;\n"
            "}\n"
            "\n"
            "QTabBar::tab:left:last, QTabBar::tab:right:last,\n"
            "QTabBar::tab:left:only-one, QTabBar::tab:right:only-one {\n"
            "    margin-bottom: 0;\n"
            "}");
    this->setMaximumWidth(parent->window()->width() / 4);
    auto mainWidget = dynamic_cast<MainWindow *>(parentWidget()->parentWidget()->parentWidget());
    connect(this, &FileBrowser::FileRename, mainWidget, &MainWindow::renameFile);
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
