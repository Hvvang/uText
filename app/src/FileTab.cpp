//
// Created by Artem Shemidko on 04.12.2020.
//

#include "FileTab.h"
#include "SaveDialog.h"
#include <QFileInfo>
#include <QPlainTextDocumentLayout>
#include <QFileDialog.h>

FileTab::FileTab(QWidget *parent)
    : QTabWidget(parent) {
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
            "    color: black;\n"
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
            "}\n"
            "\n"
            "QTabBar::tab::close-button {\n"
            "    image: url(:/clbtn/close.png)\n"
//            "    background: black\n"
            "    subcontrol-position: left;\n"
            "}"
    );
    setDocumentMode(true);
    connect(this, &FileTab::tabBarClicked, this, [=] { emit grabFocus(this);});
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(removeFileCallback(int)));

    setMovable(true);
    setTabsClosable(true);
    setUsesScrollButtons(true);
}

void FileTab::removeFileCallback(int index) {
    if (dynamic_cast<Editor *>(widget(index))->isEdited()) {
        auto dialog = new SaveDialog(tabText(index), this);
        emit grabFocus(this);

        auto result = dialog->getResult();
        if (result == 1) {
            return;
        }
        else if (result == 2) {
            emit saveFileCallback(index);
        }
    }
    tabs.remove(Path(tabText(index)));
    removeTab(index);
    if (tabs.empty()) {
        emit grabFocus(this);
        emit closePanel();
    }
}

bool FileTab::event(QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        emit grabFocus(this);
    }
    return QTabWidget::event(event);
}

void FileTab::addFile(const QString &path, Editor *widget) {
    auto label = path.mid(path.lastIndexOf("/"));

    connect(widget, &Editor::SCN_FOCUSIN, this, [=] { emit grabFocus(this);});

    if (!tabs.contains(path)) {
        tabs[path] = widget;
        insertTab(currentIndex() + 1, widget, label);
    }
    setCurrentIndex(indexOf(tabs[path]));
}

void FileTab::TabAboutToClose() {
    emit removeFileCallback(currentIndex());
}

void FileTab::TabAboutToRename(const QString &oldPath, const QString &newPath) {
    auto newName = newPath.mid(newPath.lastIndexOf("/"));

    if (tabs.contains(oldPath)) {
        tabs[newPath] = tabs[oldPath];
        tabs[newPath]->setFile(new QFile(newPath));
        tabs.remove(oldPath);
        setTabText(indexOf(tabs[newPath]), newName);
    }
}

void FileTab::TabAboutToRenameByDir(const QString &oldPath, const QString &newPath) {
    for (auto &key : tabs.keys()) {
        if (key.contains(oldPath)) {
            auto newFileName = key;
            newFileName.replace(oldPath, newPath);
            tabs[newFileName] = tabs[key];
            tabs.remove(key);
            tabs[newFileName]->setFile(new QFile(key));
        }
    }
}

void FileTab::TabAboutToSave() {
    emit saveFileCallback(currentIndex());
}

const QString &FileTab::Path(const QString &label) const {
    for(const auto &key : tabs.keys()) {
        const auto &cmp = key.mid(key.lastIndexOf('/'));
        if (cmp == label) {
            return key;
        }
    }
    return label;
}

void FileTab::saveFileCallback(int index) {
    auto file = Path(tabText(index));

    dynamic_cast<Editor *>(widget(index))->setEdited(false);
    if (file == "untitled") {
        saveFileAsCallback(file);
    }
    QFile f(file);
    if (f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        auto text = tabs[file]->text();
        f.write(text.toStdString().c_str());
        f.close();
    }
}

void FileTab::saveFileAsCallback(QString &file) {
    auto res = QFileDialog::getSaveFileName(this, tr("All Files (*)"), file);
    if (!res.isEmpty() && file != res) {
        QFile f(res);
        f.open(QIODevice::ReadWrite);
        f.close();
        emit TabAboutToRename(file, res);
        file = res;
    }
}

void FileTab::TabAboutToSaveAll() {
    for (int index = 0; index < tabs.count(); ++index) {
        emit saveFileCallback(index);
    }
}

void FileTab::TabAboutToSaveAs() {
    auto file = Path(tabText(currentIndex()));
    saveFileAsCallback(file);
    emit saveFileCallback(currentIndex());
}
