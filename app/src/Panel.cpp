//
// Created by Artem Shemidko on 07.12.2020.
//
#include "Panel.h"
#include "FileTab.h"
#include <QGridLayout>
#include <qdebug.h>
#include <QLabel>
#include <QErrorMessage>

Panel::Panel(QWidget *parent)
    : QWidget(parent) {
    setAcceptDrops(true);
}


void Panel::addPageToPanel(const QString &label, QFile *file) {

    if (!file->fileName().isEmpty() && !file->open(QIODevice::ReadOnly)) {
        QErrorMessage msg;
        msg.showMessage("Cant open file!");
        msg.exec();
        return;
    }
    file->close();
    auto widget = new QTextEdit();
    if (!file->fileName().isEmpty() && file->open(QIODevice::ReadWrite)) {
        widget->setPlainText(file->readAll());
        file->close();
    }
    if (m_lastFocus) {
        auto window = static_cast<FileTab *>(m_lastFocus);
        window->addTab(widget, label);
        window->setCurrentIndex(window->indexOf(widget));

    } else {
        auto window = new FileTab({0, 0});
        window->addTab(widget, label);
        window->setCurrentIndex(window->indexOf(widget));
        addNewWindow(rootSplitter, {0, 0}, window);
    }

}

void Panel::replaceCurrentPage(const QString &label, QFile *file) {
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


void Panel::addNewWindow(QSplitter *root, const QPair<int, int> &pos, QWidget *window) {
    auto splitH = new QSplitter(Qt::Orientation::Horizontal, root);
    auto splitV = new QSplitter(Qt::Orientation::Vertical, splitH);

    window->setParent(splitV);
    connect(static_cast<FileTab *>(window), &FileTab::grabFocus, this, &Panel::LastFocusedTabController);
    splitH->addWidget(splitV);
    splitV->addWidget(window);
    root->insertWidget(pos.second, splitH);
    LastFocusedTabController(window);
}

void Panel::setRootSplitter(QSplitter *rootSplitter) {
    this->rootSplitter = rootSplitter;
}

void Panel::LastFocusedTabController(QWidget *widget) {
    m_lastFocus = widget;
}

QWidget *Panel::copyWindow() {
    auto currTab = static_cast<FileTab *>(m_lastFocus);
    auto widget = new QTextEdit();
    auto w = new FileTab(currTab->getPos());

    widget->setPlainText(static_cast<QTextEdit *>(currTab->currentWidget())->toPlainText());
    w->addTab(widget, currTab->tabText(currTab->currentIndex()));
    return w;
}

void Panel::splitUp() {
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


void Panel::splitDown() {
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

void Panel::splitRight() {
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

void Panel::splitLeft() {
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
