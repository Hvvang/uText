//
// Created by Artem Shemidko on 04.12.2020.
//

#include "FileTab.h"
#include <QFileInfo>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>


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

//    setAcceptDrops(true);
    setMovable(true);
    setTabsClosable(true);
    setUsesScrollButtons(true);
}


void FileTab::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void FileTab::dropEvent(QDropEvent *event)
{
//    if (event->mimeData()->urls().empty()) {
//        return;
//    }
//    QString filePath = event->mimeData()->urls()[0].toLocalFile();

//    addFile(filePath);
}

void FileTab::removeFileCallback(int index) {
    tabs.remove(tabText(index));
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

void FileTab::addFile(const QString &label, QTextEdit *widget) {
    if (!tabs.contains(label)) {
        tabs[label] = widget;
        insertTab(currentIndex() + 1, widget, label);
    }
    setCurrentIndex(indexOf(tabs[label]));
}

void FileTab::TabAboutToClose() {
    emit removeFileCallback(currentIndex());
}

void FileTab::TabAboutToRename(const QString &oldPath, const QString &newPath) {
    auto oldName = oldPath.mid(oldPath.lastIndexOf("/"));
    auto newName = newPath.mid(newPath.lastIndexOf("/"));

    if (tabs.contains(oldName)) {
        tabs[newName] = tabs[oldName];
        tabs.remove(oldName);
        setTabText(indexOf(tabs[newName]), newName);
    }
}




//TextArea *FileTab::getTextArea(const QString& filename) {
//    return tab_content[filename];
//}

//TextArea *FileTab::getTextArea() {
//    int index = currentIndex();
//
//    if (index == -1)
//        return nullptr;
//
//    QString filename = getFilename(index);
//    return tab_content[filename];
//}

//QString FileTab::getFilename(int index) {
//    QString filename = tabText(index);
//
//    if (filename[filename.size() - 1] == '*') {
//        std::string std_fname = filename.toStdString();
//        std::string new_fname(std_fname.begin(), std_fname.end() - 1);
//        return QString(new_fname.c_str());
//    }
//
//    return filename;
//}
