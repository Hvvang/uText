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


    void CopyFullPathCallback(const QString &sPath);
public slots:
    void addFolderCallback(const QString &sPath);
    void ShowContextMenu(const QPoint &pos);
    void CreateFileCallback(const QString& sPath);
    void CreateFolderCallback(const QString& sPath);
    void SearchFileCallback(const QString &file);
    void SearchInFolderCallback(const QString &data);
    void removeFolderCallback(int index);
    void revealFinderCallback(const QString& sPath);

signals:
    void closeTabs();
    void AddFileProject();
    void NewFile(const QString& file);

private:
    QMap<QString, FileObserver *> tabs;

};


#endif //UTEXT_FILEBROWSER_H
