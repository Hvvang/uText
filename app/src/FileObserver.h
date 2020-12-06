//
// Created by Artem Shemidko on 10.11.2020.
//

#ifndef UTEXT_FILEOBSERVER_H
#define UTEXT_FILEOBSERVER_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeView>
#include <QEvent>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDebug>
#include <QMimeData>
#include <QApplication>
#include <QFileSystemModel>

class FileObserver : public QTreeView {
    Q_OBJECT
public:
    explicit FileObserver(QWidget *parent = Q_NULLPTR);
    void setRootPath(const QString& sPath);

protected:
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

public slots:
    void ShowContextMenu(const QPoint &pos);
    void PreCreateFileCallback(const QString& sPath);
    void PreCreateDirCallback(const QString& sPath);
    void Rename(const QString& newName);

public:
    void CreateFile(const QString& sPath);
    void CreateFolder(const QString& sPath);
    void DuplicateItem(const QString& fileName);
    void DeleteItem(const QString& file);
    void CopyItem(const QString& file);
    void CutItem(const QString& file);
    void PasteItem(const QString& file);
    void openCallback();
    void CopyPath(const QString& path);
    void RevealInFinder(const QString& file);
signals:
    void addFolderCallback(const QString& file);
private:
    QFileSystemModel *observerModel;
};


#endif //UTEXT_FILEOBSERVER_H
