//
// Created by Artem Shemidko on 07.12.2020.
//
#include "Panel.h"
#include "FileTab.h"
#include <QGridLayout>
#include <qdebug.h>
#include <QLabel>
#include <QErrorMessage>
#include "Editor.h"

[[maybe_unused]] Panel::Panel(QWidget *parent)
    : QWidget(parent) {
    setAcceptDrops(true);
    connect(this, &Panel::closeTab, this, [=] {
        if (m_lastFocus) emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToClose();
    });
    connect(this, &Panel::saveFile, this, [=] {
        if (m_lastFocus) emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToSave();
    });
    connect(this, &Panel::saveFileAs, this, [=] {
        if (m_lastFocus) emit dynamic_cast<FileTab *>(m_lastFocus)->TabAboutToSaveAs();
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
    auto widget = new Editor(); // инитилизируем редактор
     // мы же хотим читать кириллицу

//    widget->setFontPointSize(14);
//    widget->setLineWrapMode(QsciScintilla::NoWrap);

    if (!file->fileName().isEmpty() && file->open(QIODevice::ReadWrite)) {
        widget->setFile(file);
        widget->setText(file->readAll());
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
            auto editor = dynamic_cast<Editor *>(tab->currentWidget());
            tab->setTabText(tab->currentIndex(), label);

            emit editor->setFile(file);
            editor->setText(file->readAll());
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
    auto widget = new Editor();
    widget->setLex(currTab->tabText(currTab->currentIndex()));

    auto window = new FileTab(this);

    connect(dynamic_cast<FileTab *>(window), &FileTab::grabFocus, this, &Panel::LastFocusedTabController);
    connect(this, &Panel::renameTabs, dynamic_cast<FileTab *>(window), &FileTab::TabAboutToRename);
    connect(dynamic_cast<FileTab *>(window), &FileTab::closePanel, this, &Panel::closePanel);
    widget->setText(dynamic_cast<Editor *>(currTab->currentWidget())->text());
    auto path = currTab->Path(currTab->tabText(currTab->currentIndex()));
//    if (path == currTab->tabText(currTab->currentIndex())) {
//        throw ;
//    } else {
        window->addFile(path, widget);
//    }
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

void Panel::cutCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->cut();
    }
}

void Panel::copyCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->copy();
    }
}

void Panel::pasteCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->paste();
    }
}

void Panel::deleteCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->removeSelectedText();
    }
}

void Panel::selectAllCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->selectAll();
    }
}

void Panel::redoCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        if (editor->isRedoAvailable())
            editor->redo();
    }
}

void Panel::undoCallback() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        if (editor->isUndoAvailable())
            editor->undo();
    }
}

void Panel::increaseZoom() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<Editor *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->zoomIn();
    }
}

void Panel::decreaseZoom() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<QsciScintilla *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->zoomOut();
    }
}

void Panel::resetZoom() {
    if (m_lastFocus) {
        auto editor = dynamic_cast<QsciScintilla *>(dynamic_cast<FileTab *>(m_lastFocus)->currentWidget());
        editor->zoomTo(0);
    }
}

