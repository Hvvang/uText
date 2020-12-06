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
    void addFolderCallback(const QString &sPath);
    void CreateFileCallback(const QString& sPath);
    void CreateFolderCallback(const QString& sPath);
    void CopyFullPathCallback(const QString &sPath);
    void SearchFileCallback(const QString &file);
    void SearchInFolderCallback(const QString &data);


public slots:
    void ShowContextMenu(const QPoint &pos);
    void removeFolderCallback(int index);

signals:
    void closeTabs();
    void addFileProgect();

private:
    QMap<QString, FileObserver *> tabs;
};


#endif //UTEXT_FILEBROWSER_H
