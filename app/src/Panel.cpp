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
    connect(this, &Panel::closeTab, this, [=] { emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToClose(); });
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
        auto window = dynamic_cast<FileTab *>(m_lastFocus);
        window->addFile(label, widget);

    } else {
        auto window = new FileTab({0, 0});
        window->addFile(label, widget);
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
            auto tab = dynamic_cast<FileTab *>(m_lastFocus);
            tab->setTabText(tab->currentIndex(), label);
            dynamic_cast<QTextEdit *>(tab->currentWidget())->setPlainText(file->readAll());
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
    connect(dynamic_cast<FileTab *>(window), &FileTab::grabFocus, this, &Panel::LastFocusedTabController);
    connect(this, &Panel::renameTabs, dynamic_cast<FileTab *>(window), &FileTab::TabAboutToRename);
    splitH->addWidget(splitV);
    splitV->addWidget(window);
    root->insertWidget(pos.second, splitH);
    LastFocusedTabController(window);
}

void Panel::setRootSplitter(QSplitter *root) {
    this->rootSplitter = root;
}

void Panel::LastFocusedTabController(QWidget *widget) {
    m_lastFocus = widget;
}

QWidget *Panel::copyWindow() {
    auto currTab = dynamic_cast<FileTab *>(m_lastFocus);
    auto widget = new QTextEdit();
    auto w = new FileTab(currTab->getPos());

    connect(w, &FileTab::closePanel, this, &Panel::closePanel);
    widget->setPlainText(dynamic_cast<QTextEdit *>(currTab->currentWidget())->toPlainText());
    w->addFile(currTab->tabText(currTab->currentIndex()), widget);
    return w;
}

void Panel::splitUp() {
    auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
    if (root->orientation() != Qt::Vertical)
        root = dynamic_cast<QSplitter *>(root->parentWidget());
    auto pos = dynamic_cast<FileTab *>(m_lastFocus)->getPos();

    for (int i = pos.second; i < root->count(); i++) {
        auto split = dynamic_cast<FileTab *>(root->widget(i));
        split->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}

void Panel::splitDown() {
    auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
    if (root->orientation() != Qt::Vertical)
        root = dynamic_cast<QSplitter *>(root->parentWidget());
    auto pos = dynamic_cast<FileTab *>(m_lastFocus)->getPos();
    ++pos.second;

    for (int i = pos.second; i < root->count(); i++) {
        auto split = dynamic_cast<FileTab *>(root->widget(i));
        split->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}

void Panel::splitRight() {
    auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget()->parentWidget());
    auto pos = dynamic_cast<FileTab *>(m_lastFocus)->getPos();
    ++pos.second;

    for (int i = pos.second; i < root->count(); i++) {
        auto split = dynamic_cast<QSplitter *>(root->widget(i));
        auto tab = dynamic_cast<FileTab *>(split->widget(0));
        tab->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}

void Panel::splitLeft() {
    auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget()->parentWidget());
    auto pos = dynamic_cast<FileTab *>(m_lastFocus)->getPos();

    for (int i = pos.second; i < root->count(); i++) {
        auto split = dynamic_cast<QSplitter *>(root->widget(i));
        auto tab = dynamic_cast<FileTab *>(split->widget(0));
        tab->setPos({pos.first, i + 1});
    }
    addNewWindow(root, pos, copyWindow());
}

void Panel::closePanel() {
    if (rootSplitter->count())
        m_lastFocus = dynamic_cast<QSplitter *>(dynamic_cast<QSplitter *>(rootSplitter->widget(0))->widget(0))->widget(0);
    else {
        m_lastFocus = Q_NULLPTR;
    }
//    auto pos = dynamic_cast<FileTab *>(m_lastFocus)->getPos();
//    auto parent = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
//    auto child = parent;
//    while (parent->count() <= 1) {
//        child = parent;
//        parent = dynamic_cast<QSplitter *>(parent->parentWidget());
//        if (parent == rootSplitter) {
//            break;
//        }
//    }
//    if (parent == rootSplitter && rootSplitter->count() <= 1) {
//        rootSplitter->widget(0)->deleteLater();
//    } else {
//        child->deleteLater();
//    }
//    m_lastFocus = dynamic_cast<QSplitter *>(rootSplitter->widget(0))->widget(0);

}

