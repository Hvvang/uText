//
// Created by Artem Shemidko on 05.12.2020.
//

#ifndef UTEXT_FILEBROWSER_H
#define UTEXT_FILEBROWSER_H

#include <QTabWidget>
#include <qdebug.h>
#include "FileObserver.h"

class FileBrowser : public QTabWidget {
Q_OBJECT
public:
    explicit FileBrowser(QWidget* parent = nullptr);
    void addFolder(const QString &sPath);
    void CreateFile(const QString& sPath);
    void CreateFolder(const QString& sPath);
    void CopyFullPath(const QString &sPath);


public slots:
    void ShowContextMenu(const QPoint &pos);
    void removeFolder(int index);

signals:
    void closeTabs();
    void addFileProgect();

private:
    QMap<QString, FileObserver *> tabs;
};


#endif //UTEXT_FILEBROWSER_H
