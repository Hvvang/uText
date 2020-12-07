#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "FileObserver.h"
#include "PopupDialog.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QString sPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->textEdit->setHidden(true);
    ui->BrowserView->setHidden(true);
    ui->preview->setMaximumWidth(window()->width() / 4);
    ui->Panel->setRootSplitter(ui->RootSplitter);

    connect(ui->addFolderBtn, &QPushButton::released, this, &MainWindow::openCallback);
    connect(ui->BrowserView, &FileBrowser::AddFileProject, this, &MainWindow::openCallback);
    connect(ui->BrowserView, &FileBrowser::customContextMenuRequested, this, &MainWindow::addContextMenuForBrowser);
    connect(ui->BrowserView, &FileBrowser::closeTabs, this, &MainWindow::browserSwitch);
    connect(this, &MainWindow::closeTabs, this, &MainWindow::browserSwitch);
    connect(this, &MainWindow::createFile, ui->BrowserView, &FileBrowser::CreateFileCallback);
    connect(this, &MainWindow::createFolder, ui->BrowserView, &FileBrowser::CreateFolderCallback);
    connect(this, &MainWindow::openDialog, this, &MainWindow::openCallback);
    connect(this, &MainWindow::removeTab, ui->BrowserView, &FileBrowser::removeFolderCallback);
    connect(this, &MainWindow::copyPath, ui->BrowserView, &FileBrowser::CopyFullPathCallback);
    connect(this, &MainWindow::searchFile, ui->BrowserView, &FileBrowser::SearchFileCallback);
    connect(this, &MainWindow::searchInFolder, ui->BrowserView, &FileBrowser::SearchInFolderCallback);
    connect(this, &MainWindow::revealInFinder, ui->BrowserView, &FileBrowser::revealFinderCallback);

//    connect(ui->BrowserView, &FileBrowser::oneClick, this, &MainWindow::replaceFileInView);
    connect(ui->BrowserView, &FileBrowser::doubleClick, this, &MainWindow::addFileToView);
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
    connect(ui->actionSplit_Up, &QAction::triggered, ui->Panel, &Test::splitUp);
    connect(ui->actionSplit_Down, &QAction::triggered, ui->Panel, &Test::splitDown);
    connect(ui->actionSplit_Right, &QAction::triggered, ui->Panel, &Test::splitRight);
    connect(ui->actionSplit_Left, &QAction::triggered, ui->Panel, &Test::splitLeft);

}

void MainWindow::openCallback() {
    QFileDialog dialog;
    dialog.setOptions(QFileDialog::DontResolveSymlinks);
    dialog.setNameFilter(tr("Open..."));
    if (dialog.exec() == QDialog::Accepted) {
        QString dirName = dialog.selectedFiles().front();
        if (QDir(dirName).isReadable()) {
            ui->BrowserView->addFolderCallback(dirName);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Not enough permission!"), QMessageBox::Ok);
        }
    }
}

void MainWindow::newFileCallback(const QString& sPath) {
//  create new tab in panel
    emit createFile(sPath);

}

void MainWindow::newFolderCallback(const QString& sPath) {
//  create new tab in panel
    emit createFolder(sPath);
}

void MainWindow::newCallback() {
//  create new tab in panel
    ui->Panel->addPageToPanel("untitled", new QFile());
}

void MainWindow::saveCallback() {

}

void MainWindow::saveAsCallback() {

}

void MainWindow::saveAllCallback() {

}

void MainWindow::closeCallback() {

}

void MainWindow::browserSwitch() {
    auto browser = ui->BrowserView;
    auto preview = ui->preview;

    preview->isHidden() ? preview->show() : preview->hide();
    browser->isHidden() ? browser->show() : browser->hide();
}

void MainWindow::addContextMenuForBrowser(const QPoint &pos) {
    auto dialog = new PopupDialog(this);
    QMenu contextMenu(tr("Context menu"), ui->BrowserView);
    QAction action1(tr("New File"), this);
    QAction action2(tr("New Folder"), this);
    QAction action3(tr("Add Project Folder"), this);
    QAction action4(tr("Remove Project Folder"), this);
    QAction action5(tr("Copy Full Path"), this);
    QAction action6(tr("Search File"), this);
    QAction action7(tr("Search in Folder"), this);
    QAction action8(tr("Reveal in Finder"), this);

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);
    contextMenu.addAction(&action7);
    contextMenu.addAction(contextMenu.addSeparator());
    contextMenu.addAction(&action8);

    connect(&action1, &QAction::triggered, this, [=] {
        connect(dialog, &PopupDialog::NewFile, this, &MainWindow::newFileCallback);
        dialog->setParams("Enter the path for the new file.", "", Type::NewFile);
    });
    connect(&action2, &QAction::triggered, this, [=] {
        connect(dialog, &PopupDialog::NewFolder, this, &MainWindow::newFolderCallback);
        dialog->setParams("Enter the path for the new folder.", "", Type::NewDir);
    });
    connect(&action3, &QAction::triggered, this, [this] {
        emit openDialog();
    });
    connect(&action4, &QAction::triggered, ui->BrowserView, [=] {
        auto browser = ui->BrowserView;
        auto index = (browser->tabBar()->tabAt(pos) != -1)
                ? browser->tabBar()->tabAt(pos)
                : browser->currentIndex();
        emit removeTab(index);
    });
    connect(&action5, &QAction::triggered, this, [=] {
        auto browser = ui->BrowserView;
        auto indexText = (browser->tabBar()->tabAt(pos) != -1)
                     ? browser->tabText(browser->tabBar()->tabAt(pos))
                     : browser->tabText(browser->currentIndex());
        emit copyPath(indexText);
    });
    connect(&action6, &QAction::triggered, this, [=] {
        auto browser = ui->BrowserView;
        if (browser->tabBar()->tabAt(pos) != -1)
            browser->setCurrentIndex(browser->tabBar()->tabAt(pos));
        connect(dialog, &PopupDialog::SearchFile, this, &MainWindow::searchFile);
        dialog->setParams("Enter the path for the search file.", "", Type::SearchFile);
    });
    connect(&action7, &QAction::triggered, this, [=] {
        connect(dialog, &PopupDialog::SearchInDir, this, &MainWindow::searchInFolder);
        dialog->setParams("Enter smth you want to find in files.", "", Type::SearchInDir);
    });
    connect(&action8, &QAction::triggered, this, [=] {
        auto browser = ui->BrowserView;
        auto indexText = (browser->tabBar()->tabAt(pos) != -1)
                         ? browser->tabText(browser->tabBar()->tabAt(pos))
                         : browser->tabText(browser->currentIndex());
        auto dirPath = static_cast<QFileSystemModel *>(browser->Tabs()[indexText]->model())->filePath(browser->Tabs()[indexText]->rootIndex());
        emit revealInFinder(dirPath);
    });
    contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}
void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}
void MainWindow::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls()) {
        return;
    }
    auto droppedData = event->mimeData()->urls();
    if (droppedData.count() == 1) {
        auto dataName = droppedData.at(0).toString().remove(0, 7);
        QFileInfo info(dataName);
        if (info.isFile()) {
//            ui->FileTab->addFile(dataName);
        } else {
            auto dir = QDir(dataName);
            ui->BrowserView->addFolderCallback(dir.canonicalPath());
        }
    }
}

void MainWindow::replaceFileInView(const QString &sPath) {
    auto label = sPath;

    ui->Panel->replaceCurrentPage(label.remove(0, label.lastIndexOf('/')), new QFile(sPath));
}

void MainWindow::addFileToView(const QString &sPath) {
    auto label = sPath;

    ui->Panel->addPageToPanel(label.remove(0, label.lastIndexOf('/')), new QFile(sPath));
}