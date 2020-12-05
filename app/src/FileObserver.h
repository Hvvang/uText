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
private:
    void CreateFile(QString path);
    void CreateFolder(QString path);
    void Rename(QString file);
    void Delete(QString file);


private:
    QFileSystemModel *observerModel;
};


#endif //UTEXT_FILEOBSERVER_H
