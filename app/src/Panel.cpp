//
// Created by Artem Shemidko on 07.12.2020.
//
#include "Panel.h"
#include "FileTab.h"
#include <QGridLayout>
#include <qdebug.h>
#include <QLabel>
#include <QErrorMessage>

[[maybe_unused]] Panel::Panel(QWidget *parent)
    : QWidget(parent) {
    setAcceptDrops(true);
    connect(this, &Panel::closeTab, this, [=] {
        if (m_lastFocus) emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToClose();
    });
    connect(this, &Panel::saveFile, this, [=] {
        if (m_lastFocus) emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToSave();
    });
    connect(this, &Panel::saveAllFiles, this, [=] {
        if (m_lastFocus) emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToSaveAll();
    });
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
        auto window = new FileTab();
        window->addFile(label, widget);
        rootSplitter->addWidget(window);
        connect(dynamic_cast<FileTab *>(window), &FileTab::grabFocus, this, &Panel::LastFocusedTabController);
        connect(this, &Panel::renameTabs, dynamic_cast<FileTab *>(window), &FileTab::TabAboutToRename);
        connect(dynamic_cast<FileTab *>(window), &FileTab::closePanel, this, &Panel::closePanel);
        LastFocusedTabController(window);
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

void Panel::setRootSplitter(QSplitter *root) {
    this->rootSplitter = root;
}

void Panel::LastFocusedTabController(QWidget *widget) {
    m_lastFocus = widget;
}

QWidget *Panel::copyWindow() {
    auto currTab = dynamic_cast<FileTab *>(m_lastFocus);
    auto widget = new QTextEdit();
    auto window = new FileTab(this);

    connect(dynamic_cast<FileTab *>(window), &FileTab::grabFocus, this, &Panel::LastFocusedTabController);
    connect(this, &Panel::renameTabs, dynamic_cast<FileTab *>(window), &FileTab::TabAboutToRename);
    connect(dynamic_cast<FileTab *>(window), &FileTab::closePanel, this, &Panel::closePanel);
    widget->setPlainText(dynamic_cast<QTextEdit *>(currTab->currentWidget())->toPlainText());
    auto path = currTab->Path(currTab->tabText(currTab->currentIndex()));
    if (path == currTab->tabText(currTab->currentIndex())) {
        throw ;
    } else {
        window->addFile(path, widget);
    }
    return window;
}

void Panel::splitUp() {
    if (m_lastFocus) {
        auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
        auto splitV = new QSplitter(Qt::Orientation::Vertical, this);

        auto pos = root->indexOf(m_lastFocus);
        auto w = copyWindow();
        w->setParent(splitV);
        auto widget = root->replaceWidget(pos, splitV);
        splitV->setParent(root);
        splitV->addWidget(w);
        splitV->addWidget(widget);
        auto sizes = splitV->sizes();
        for (auto &s : sizes) {
            s = root->size().height() / root->count();
        }
        splitV->setSizes(sizes);
        LastFocusedTabController(w);
    }
}

void Panel::splitDown() {
    if (m_lastFocus) {
        auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
        auto splitV = new QSplitter(Qt::Orientation::Vertical, this);

        auto pos = root->indexOf(m_lastFocus);
        auto w = copyWindow();
        w->setParent(splitV);
        auto widget = root->replaceWidget(pos, splitV);
        splitV->setParent(root);
        splitV->addWidget(widget);
        splitV->addWidget(w);
        auto sizes = splitV->sizes();
        for (auto &s : sizes) {
            s = root->size().height() / root->count();
        }
        splitV->setSizes(sizes);
        LastFocusedTabController(w);
    }
}

void Panel::splitRight() {
    if (m_lastFocus) {
        auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
        auto splitV = new QSplitter(Qt::Orientation::Horizontal, this);

        auto pos = root->indexOf(m_lastFocus);
        auto w = copyWindow();
        w->setParent(splitV);
        auto widget = root->replaceWidget(pos, splitV);
        splitV->setParent(root);
        splitV->addWidget(widget);
        splitV->addWidget(w);
        auto sizes = splitV->sizes();
        for (auto &s : sizes) {
            s = root->size().width() / root->count();
        }
        splitV->setSizes(sizes);
        LastFocusedTabController(w);
    }
}

void Panel::splitLeft() {
    if (m_lastFocus) {
        auto root = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
        auto splitV = new QSplitter(Qt::Orientation::Horizontal, this);

        auto pos = root->indexOf(m_lastFocus);
        auto w = copyWindow();
        w->setParent(splitV);
        auto widget = root->replaceWidget(pos, splitV);
        splitV->setParent(root);
        splitV->addWidget(w);
        splitV->addWidget(widget);
        auto sizes = splitV->sizes();
        for (auto &s : sizes) {
            s = root->size().width() / root->count();
        }
        splitV->setSizes(sizes);
        LastFocusedTabController(w);
    }
}

void Panel::closePanel() {
    auto parent = dynamic_cast<QSplitter *>(m_lastFocus->parentWidget());
    auto child = m_lastFocus;

    while (parent->count() == 1) {
        if (parent == rootSplitter) {
            delete child;
            m_lastFocus = Q_NULLPTR;
            return;
        }
        child = parent;
        parent = dynamic_cast<QSplitter *>(parent->parentWidget());
    }
    auto index = (parent->indexOf(child) - 1 > 0) ? parent->indexOf(child) - 1 : 0;
    delete child;

    m_lastFocus = parent->widget(index);
    while (m_lastFocus->metaObject()->className() == rootSplitter->metaObject()->className()) {
        m_lastFocus = dynamic_cast<QSplitter *>(m_lastFocus)->widget(0);
    }
}


