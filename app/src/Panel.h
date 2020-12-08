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
    [[maybe_unused]] explicit Panel(QWidget *parent = Q_NULLPTR);

    void setRootSplitter(QSplitter *rootSpliter);
    void splitUp();
    void splitDown();
    void splitLeft();
    void splitRight();
    void replaceCurrentPage(const QString& label, QFile *file);
    void addPageToPanel(const QString& label, QFile *file);

public slots:
    void LastFocusedTabController(QWidget *widget);
    void closePanel();

    void redoCallback();
    void undoCallback();
    void cutCallback();
    void copyCallback();
    void pasteCallback();
    void deleteCallback();
    void selectAllCallback();

    void increaseZoom();
    void decreaseZoom();
    void resetZoom();

signals:
    void closeTab();
    void saveFile();
    void saveFileAs();
    void saveAllFiles();
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
};


#endif //UTEXT_PANEL_H
