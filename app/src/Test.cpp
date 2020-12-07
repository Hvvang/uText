//
// Created by Artem Shemidko on 07.12.2020.
//
#include "Test.h"
#include "FileTab.h"
#include <QGridLayout>
#include <qdebug.h>
#include <QLabel>
#include <QErrorMessage>

Test::Test(QWidget *parent)
    : QWidget(parent) {
    setAcceptDrops(true);
}

Test::~Test() {
//    delete m_split;
}

void Test::addPageToPanel(const QString &label, QFile *file) {
    if (!file->open(QIODevice::ReadOnly)) {
        QErrorMessage msg;
        msg.showMessage("Cant open file!");
        msg.exec();
        return;
    }
    file->close();
    auto widget = new QTextEdit();
    if (file->open(QIODevice::ReadWrite)) {
        widget->setPlainText(file->readAll());
        file->close();
    }

    if (m_lastFocus) {
        static_cast<FileTab *>(m_lastFocus)->addTab(widget, label);

    } else {
        auto window = new FileTab({0, 0});
        window->addTab(widget, label);
        addNewWindow(rootSplitter, {0, 0}, window);
    }

}

void Test::replaceCarrentPage(const QString &label, QFile *file) {
    if (m_lastFocus) {
        if (!file->open(QIODevice::ReadOnly)) {
            QErrorMessage msg;
            msg.showMessage("Cant open file!");
            msg.exec();
            return;
        }
        file->close();
        if (file->open(QIODevice::ReadWrite)) {
            auto tab = static_cast<FileTab *>(m_lastFocus);
            tab->setTabText(tab->currentIndex(), label);
            static_cast<QTextEdit *>(tab->currentWidget())->setPlainText(file->readAll());
            file->close();
        }
    } else {
        addPageToPanel(label, file);
    }
}


void Test::addNewWindow(QSplitter *root, const QPair<int, int> &pos, QWidget *window) {
    auto splitH = new QSplitter(Qt::Orientation::Horizontal, root);
    auto splitV = new QSplitter(Qt::Orientation::Vertical, splitH);

    window->setParent(splitV);
    connect(static_cast<FileTab *>(window), &FileTab::grabFocus, this, &Test::LastFocusedTabController);
    splitH->addWidget(splitV);
    splitV->addWidget(window);
    root->insertWidget(pos.second, splitH);
    LastFocusedTabController(window);
}

void Test::setRootSplitter(QSplitter *rootSplitter) {
    this->rootSplitter = rootSplitter;
}

void Test::LastFocusedTabController(QWidget *widget) {
    m_lastFocus = widget;
    qDebug() << widget;
//    auto tab = static_cast<FileTab *>(m_lastFocus);
//    qDebug() << tab->getPos() << "  " << static_cast<QSplitter *>(tab->parentWidget())->orientation();

}

QWidget *Test::copyWindow() {
    auto currTab = static_cast<FileTab *>(m_lastFocus);
    auto widget = new QTextEdit();
    auto w = new FileTab(currTab->getPos());

    widget->setPlainText(static_cast<QTextEdit *>(currTab->currentWidget())->toPlainText());
    w->addTab(widget, currTab->tabText(currTab->currentIndex()));
    return w;
}

void Test::splitUp() {
    auto root = static_cast<QSplitter *>(m_lastFocus->parentWidget());
    if (root->orientation() != Qt::Vertical)
        root = static_cast<QSplitter *>(root->parentWidget());
    auto pos = static_cast<FileTab *>(m_lastFocus)->getPos();

    for (int i = pos.second; i < root->count(); i++) {
        auto split = static_cast<FileTab *>(root->widget(i));
        split->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}


void Test::splitDown() {
    auto root = static_cast<QSplitter *>(m_lastFocus->parentWidget());
    if (root->orientation() != Qt::Vertical)
        root = static_cast<QSplitter *>(root->parentWidget());
    auto pos = static_cast<FileTab *>(m_lastFocus)->getPos();
    ++pos.second;

    for (int i = pos.second; i < root->count(); i++) {
        auto split = static_cast<FileTab *>(root->widget(i));
        split->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}

void Test::splitRight() {
    auto root = static_cast<QSplitter *>(m_lastFocus->parentWidget());
    if (root->orientation() != Qt::Horizontal)
        root = static_cast<QSplitter *>(root->parentWidget());
    auto pos = static_cast<FileTab *>(m_lastFocus)->getPos();
    ++pos.second;

    for (int i = pos.second; i < root->count(); i++) {
        auto split = static_cast<QSplitter *>(root->widget(i));
        auto tab = static_cast<FileTab *>(split->widget(0));
        tab->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}

void Test::splitLeft() {
    auto root = static_cast<QSplitter *>(m_lastFocus->parentWidget());
    if (root->orientation() != Qt::Horizontal)
        root = static_cast<QSplitter *>(root->parentWidget());
    auto pos = static_cast<FileTab *>(m_lastFocus)->getPos();

    for (int i = pos.second; i < root->count(); i++) {
        auto split = static_cast<QSplitter *>(root->widget(i));
        auto tab = static_cast<FileTab *>(split->widget(0));
        tab->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}
