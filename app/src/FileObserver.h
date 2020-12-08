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

public slots:
    void ShowContextMenu(const QPoint &pos);
    void PreCreateFileCallback(const QString& sPath);
    void PreCreateDirCallback(const QString& sPath);
    void Rename(const QString& newName);
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    static void CreateFile(const QString& sPath);
    static void CreateFolder(const QString& sPath);
    static void DuplicateItem(const QString& fileName);
    static void DeleteItem(const QString& file);
    static void CopyItem(const QString& file);
    static void CutItem(const QString& file);
    void static PasteItem(const QString& file);
    void openCallback();
    static void CopyPath(const QString& path);
    void RevealInFinder(const QString& file);
signals:
    void oneClick(const QString &sPath);
    void doubleClick(const QString &sPath);

private:
    QFileSystemModel *observerModel;
};


#endif //UTEXT_FILEOBSERVER_H
