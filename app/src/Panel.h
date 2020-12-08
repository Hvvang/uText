//
// Created by Artem Shemidko on 07.12.2020.
//

#ifndef UTEXT_PANEL_H
#define UTEXT_PANEL_H

#include <QWidget>
#include <QFile>
#include <QSplitter>
#include <QTabWidget>
#include "FileTab.h"

class Panel : public QWidget {
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = Q_NULLPTR);

    void setRootSplitter(QSplitter *rootSpliter);
    void splitUp();
    void splitDown();
    void splitLeft();
    void splitRight();
    void addPageToPanel(const QString& label, QFile *file);
    void replaceCurrentPage(const QString& label, QFile *file);
public slots:
    void addNewWindow(QSplitter *root, const QPair<int, int> &pos, QWidget *window);
    void LastFocusedTabController(QWidget *widget);
    void closePanel();
signals:
    void closeTab();
    void renameTabs(const QString &oldPath, const QString &newPath);


private:
    QWidget *copyWindow();
//    void dragMoveEvent(QDragMoveEvent *event) override;
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dropEvent(QDropEvent *event) override;
//protected:
//    void LastFocusedTabController(QWidget *widget);
//    void AddPageToLastFocus(QString label, QFile *file);
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
private:
    QWidget *m_lastFocus{Q_NULLPTR};
    QSplitter *rootSplitter{Q_NULLPTR};
    QVector<QWidget *> w;
};


#endif //UTEXT_PANEL_H
