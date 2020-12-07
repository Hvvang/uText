//
// Created by Artem Shemidko on 04.12.2020.
//

#include "FileTab.h"
#include <QFileInfo>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>


FileTab::FileTab(QPair<int, int> pos, QWidget *parent)
    : QTabWidget(parent)
    , pos(pos) {



//    setAcceptDrops(true);
//    setMovable(true);
//    setTabsClosable(true);
//    setUsesScrollButtons(true);
//    QObject::connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
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

//void FileTab::addFile(QString filePath, Ui::MainWindow *ui) {
//    if (tab_content.count(filePath)) {
//        setCurrentIndex(indexOf(tab_content[filePath]));
//        return;
//    }
//
//    QFileInfo info(filePath);
//
//    if (info.isFile()) {
//        auto area = ui->textEdit;
//        QFile *file = new QFile(filePath);
//
//        if (file->open(QIODevice::ReadWrite)) {
//            QTextStream in(file);
//            QTextDocument *txtDoc = new QTextDocument(area);
//
//            txtDoc->setDocumentLayout(new QPlainTextDocumentLayout(txtDoc));
//            txtDoc->setPlainText(in.readAll());
//            area->setDocument(txtDoc);
//            setCurrentIndex(addTab(area, filePath));
//            emit currentChanged(currentIndex());
//            setTabToolTip(currentIndex(), filePath);
//            QObject::connect(area, SIGNAL(textChanged()), SLOT(updateTabName()));
//
//            tab_content[filePath] = area;
//        }
//    }
//}

void FileTab::renameFile(QString oldPath, QString newPath) {
//    if (tab_content.count(oldPath)) {
//        tab_content[newPath] = tab_content[oldPath];
//        tab_content.erase(oldPath);
//
//        QString tabtext = tabText(indexOf(tab_content[newPath]));
//        if (tabtext[tabtext.size() - 1] == '*')
//            setTabText(indexOf(tab_content[newPath]), newPath + "*");
//        else
//            setTabText(indexOf(tab_content[newPath]), newPath);
//
//        setTabToolTip(indexOf(tab_content[newPath]), newPath);
//    }
}

void FileTab::removeFile(QString filePath) {
//    if (tab_content.count(filePath)) {
//        emit tabCloseRequested(indexOf(tab_content[filePath]));
//        tab_content.erase(filePath);
//    }
}

void FileTab::closeTab(int index) {
//    tab_content.erase(tabText(index));
    removeTab(index);
}

void FileTab::updateTabName() {
    int index = currentIndex();

    if (index == -1)
        return;

    QString tab_text = tabText(index);

    if (tab_text[tab_text.size() - 1] == '*')
        return;

    setTabText(index, tab_text + "*");
}

const QPair<int, int> &FileTab::getPos() const {
    return pos;
}

const int &FileTab::splitterPos() const {
    return pos.first;
}

const int &FileTab::indexPos() const {
    return pos.second;
}

void FileTab::setPos(const QPair<int, int> &pos) {
    this->pos = pos;
}

bool FileTab::event(QEvent *event) {
//    qDebug() << event;
    if (event->type() == QEvent::MouseButtonRelease) {
        emit grabFocus(this);
    }
    return QTabWidget::event(event);
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

