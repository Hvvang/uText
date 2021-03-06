#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "FileObserver.h"
#include "PopupDialog.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(const QString &sPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->BrowserView->setHidden(true);
    ui->preview->setMaximumWidth(window()->width() / 4);
    ui->preview->setMinimumWidth(window()->width() / 6);
    ui->BrowserView->setMinimumWidth(0);
    ui->SplitPanel->setRootSplitter(ui->RootSplitter);
    ui->toolBar->hide();

    dialog = new PopupDialog(this);

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

    if (sPath.isEmpty()) {
        newCallback();
    } else {
        QDir dir(sPath);
        if (dir.isReadable()) {
            ui->BrowserView->addFolderCallback(dir.canonicalPath());
        } else {
            addFileToView(sPath);
        }
    }

    menuConnector();
}

MainWindow::~MainWindow() {
    delete ui;
    if (dialog) {
        delete dialog;
    }
}

void MainWindow::menuConnector() {
    connect(ui->actionNew_Window, &QAction::triggered, this, [=] { emit newWindowCallback(""); });
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newCallback);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openCallback);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveCallback);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAsCallback);
    connect(ui->actionSave_all, &QAction::triggered, this, &MainWindow::saveAllCallback);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeCallback);


    connect(ui->actionRedo, &QAction::triggered, ui->SplitPanel, &Panel::redoCallback);
    connect(ui->actionUndo, &QAction::triggered, ui->SplitPanel, &Panel::undoCallback);
    connect(ui->actionCut, &QAction::triggered, ui->SplitPanel, &Panel::cutCallback);
    connect(ui->actionCopy, &QAction::triggered, ui->SplitPanel, &Panel::copyCallback);
    connect(ui->actionPaste, &QAction::triggered, ui->SplitPanel, &Panel::pasteCallback);
    connect(ui->actionDelete, &QAction::triggered, ui->SplitPanel, &Panel::deleteCallback);
    connect(ui->actionSelect_All, &QAction::triggered, ui->SplitPanel, &Panel::selectAllCallback);

    connect(ui->actionSplit_Up, &QAction::triggered, ui->SplitPanel, &Panel::splitUp);
    connect(ui->actionSplit_Down, &QAction::triggered, ui->SplitPanel, &Panel::splitDown);
    connect(ui->actionSplit_Right, &QAction::triggered, ui->SplitPanel, &Panel::splitRight);
    connect(ui->actionSplit_Left, &QAction::triggered, ui->SplitPanel, &Panel::splitLeft);

    connect(ui->actionIncrease_Font_Size, &QAction::triggered, ui->SplitPanel, &Panel::increaseZoom);
    connect(ui->actionDecrease_Font_Size, &QAction::triggered, ui->SplitPanel, &Panel::decreaseZoom);
    connect(ui->actionReset_Font_Size, &QAction::triggered, ui->SplitPanel, &Panel::resetZoom);

    connect(ui->actionShow_Tab_Bar, &QAction::triggered, ui->toolBar, [=] {
        if (ui->toolBar->isHidden()) { ui->toolBar->show(); ui->actionShow_Tab_Bar->setText("Hide Tool Bar"); }
        else { ui->toolBar->hide(); ui->actionShow_Tab_Bar->setText("Show Tool Bar"); }
    });
    connect(ui->actionToggle_Tree_View, &QAction::triggered, this, &MainWindow::toggleBrowser);

    connect(ui->actionFind_in_File, &QAction::triggered, this, [=] {
        auto dialog = new PopupDialog(this);
        connect(dialog, &PopupDialog::Find, ui->SplitPanel, &Panel::find);
        dialog->setParams("Find in file.", "", Type::Find);
        delete dialog;
    });
    connect(ui->actionFind_Next, &QAction::triggered, ui->SplitPanel, &Panel::findNext);
    connect(ui->actionFind_Previous, &QAction::triggered, ui->SplitPanel, &Panel::findPrev);
    connect(ui->actionReplace_in_File, &QAction::triggered, this, [=] {
        auto dialog = new PopupDialog(this);
        connect(dialog, &PopupDialog::Replace, ui->SplitPanel, &Panel::replace);
        dialog->setParams("Replace in file.", "", Type::Replace);
        delete dialog;
    });

}

void MainWindow::openCallback() {
    QFileDialog d;
    d.setFileMode(QFileDialog::AnyFile);
    d.setOptions(QFileDialog::DontResolveSymlinks);
    if (d.exec() == QDialog::Accepted) {
        QString path = d.selectedFiles().front();
        QString ext = QFileInfo(path).completeSuffix();
        QFileInfo info(path);
        if (info.isFile() && info.isReadable())
            ui->SplitPanel->addPageToPanel(path, new QFile(path));
        else if (info.isDir() && info.isReadable())
            ui->BrowserView->addFolderCallback(path);
        else
            QMessageBox::warning(this, tr("Error"), tr("Not enough permission!"), QMessageBox::Ok);
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
    ui->SplitPanel->addPageToPanel("untitled", new QFile());
}

void MainWindow::saveCallback() {
    emit ui->SplitPanel->saveFile();
}

void MainWindow::saveAsCallback() {
    emit ui->SplitPanel->saveFileAs();
}

void MainWindow::saveAllCallback() {
    emit ui->SplitPanel->saveAllFiles();
}

void MainWindow::closeCallback() {
    emit ui->SplitPanel->closeTab();
}

void MainWindow::renameFile(const QString &oldPath, const QString &newPath) {
    emit ui->SplitPanel->renameTabs(oldPath, newPath);
}

void MainWindow::renameFileByDirRename(const QString &oldPath, const QString &newPath) {
    emit ui->SplitPanel->renameTabsByDir(oldPath, newPath);
}

void MainWindow::browserSwitch() {
    auto browser = ui->BrowserView;
    auto preview = ui->preview;

    preview->isHidden() ? preview->show() : preview->hide();
    browser->isHidden() ? browser->show() : browser->hide();
}

void MainWindow::addContextMenuForBrowser(const QPoint &pos) {
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
        connect(dynamic_cast<PopupDialog *>(dialog), &PopupDialog::NewFile, this, &MainWindow::newFileCallback);
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter the path for the new file.", "", Type::NewFile);
    });
    connect(&action2, &QAction::triggered, this, [=] {
        connect(dynamic_cast<PopupDialog *>(dialog), &PopupDialog::NewFolder, this, &MainWindow::newFolderCallback);
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter the path for the new folder.", "", Type::NewDir);
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
        connect(dynamic_cast<PopupDialog *>(dialog), &PopupDialog::SearchFile, this, &MainWindow::searchFile);
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter the path for the search file.", "", Type::SearchFile);
    });
    connect(&action7, &QAction::triggered, this, [=] {
        connect(dynamic_cast<PopupDialog *>(dialog), &PopupDialog::SearchInDir, this, &MainWindow::searchInFolder);
        dynamic_cast<PopupDialog *>(dialog)->setParams("Enter smth you want to find in files.", "", Type::SearchInDir);
    });
    connect(&action8, &QAction::triggered, this, [=] {
        auto browser = ui->BrowserView;
        auto indexText = (browser->tabBar()->tabAt(pos) != -1)
                         ? browser->tabText(browser->tabBar()->tabAt(pos))
                         : browser->tabText(browser->currentIndex());
        auto dirPath = dynamic_cast<QFileSystemModel *>(browser->Tabs()[indexText]->model())->filePath(browser->Tabs()[indexText]->rootIndex());
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
            ui->SplitPanel->addPageToPanel(dataName, new QFile(dataName));
        } else {
            auto dir = QDir(dataName);
            ui->BrowserView->addFolderCallback(dir.canonicalPath());
        }
    }
}

[[maybe_unused]] void MainWindow::replaceFileInView(const QString &sPath) {
    auto label = sPath;

    ui->SplitPanel->replaceCurrentPage(label.remove(0, label.lastIndexOf('/')), new QFile(sPath));
}

void MainWindow::addFileToView(const QString &sPath) {
    auto label = sPath;

    ui->SplitPanel->addPageToPanel(label, new QFile(sPath));
}

void MainWindow::toggleBrowser() {
    if (ui->preview->isHidden() && !ui->BrowserView->currentWidget()->isHidden()) {
        ui->BrowserView->currentWidget()->hide();
        auto width = ui->BrowserView->width();
        auto vSize = ui->splitter->sizes();

        vSize[1] -= width;
        vSize[2] = ui->splitter->width() - ui->BrowserView->tabBar()->width();
        ui->splitter->setSizes(vSize);
        treeSize = width;
    }
    else if (ui->preview->isHidden()) {
        auto vSize = ui->splitter->sizes();
        ui->BrowserView->currentWidget()->show();

        vSize[1] += treeSize;
        vSize[2] -= treeSize;
        ui->splitter->setSizes(vSize);
    }
}
