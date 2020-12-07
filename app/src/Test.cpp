//
// Created by Artem Shemidko on 07.12.2020.
//
#include "Test.h"
#include "FileTab.h"
#include <QGridLayout>
#include <qdebug.h>
#include <QLabel>

Test::Test(QWidget *parent)
    : QWidget(parent) {
    setAcceptDrops(true);
}

Test::~Test() {
//    delete m_split;
}

void Test::addPageToPanel(const QString &label, QFile *file) {
    if (m_lastFocus) {

    } else {

    }

}

void Test::addNewWindow(QSplitter *root, const QPair<int, int> &pos) {
    auto splitH = new QSplitter(Qt::Orientation::Horizontal, root);
    auto splitV = new QSplitter(Qt::Orientation::Vertical, splitH);

    auto window = new FileTab(pos, splitV);

    connect(window, &FileTab::grabFocus, this, &Test::LastFocusedTabController);
    window->addTab(new QWidget(), "page inserted" + QString::number(pos.second));

    splitH->addWidget(splitV);
    splitV->addWidget(window);
    root->insertWidget(pos.second, splitH);
    LastFocusedTabController(window);
//    auto split = new QSplitter(Qt::Orientation::Vertical, rootSplitter);
//    auto window = new FileTab({splitterPos, indexPos}, split);
//
//    LastFocusedTabController(window);
//    connect(window, &FileTab::grabFocus, this, &Test::LastFocusedTabController);
//    window->addTab(new QWidget(), "page" + QString::number(indexPos));
//    rootSplitter->insertWidget(indexPos, split);
//    split->addWidget(window);
}

void Test::setRootSplitter(QSplitter *rootSplitter) {
    this->rootSplitter = rootSplitter;
}

void Test::LastFocusedTabController(QWidget *widget) {
    m_lastFocus = widget;
    auto tab = static_cast<FileTab *>(m_lastFocus);
    qDebug() << tab->getPos() << "  " << static_cast<QSplitter *>(tab->parentWidget())->orientation();

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
    addNewWindow(root, pos);
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
    addNewWindow(root, pos);
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
    addNewWindow(root, pos);
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
    addNewWindow(root, pos);
}
