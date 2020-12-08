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
    [[maybe_unused]] explicit FileBrowser(QWidget* parent = nullptr);

public slots:
    void addFolderCallback(const QString &sPath);
    void CreateFileCallback(const QString& sPath);
    void CreateFolderCallback(const QString& sPath);
    void SearchFileCallback(const QString &file);
    static void SearchInFolderCallback(const QString &data);
    void removeFolderCallback(int index);
    static void revealFinderCallback(const QString& sPath);
    void CopyFullPathCallback(const QString &sPath);
    void oneClickCallback(const QString &sPath);
    void doubleClickCallback(const QString &sPath);
    [[nodiscard]] const QMap<QString, FileObserver *> &Tabs() const;

signals:
    void closeTabs();
    void AddFileProject();
//    void NewFile(const QString& file);
    void oneClick(const QString &sPath);
    void doubleClick(const QString &sPath);
    void FileRename(const QString &oldPath, const QString &newPath);

private:
    QMap<QString, FileObserver *> tabs;
};


#endif //UTEXT_FILEBROWSER_H
