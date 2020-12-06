#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QString sPath = "/", QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addContextMenuForBrowser(const QPoint &pos);
    void newFileCallback(const QString& sPath);
    void newFolderCallback(const QString& sPath);

public:
    void menuConnector();
    void newCallback();
    void openCallback();
    void saveCallback();
    void saveAsCallback();
    void saveAllCallback();
    void closeCallback();
    void browserSwitch();

signals:
    void createFile(const QString& sPath);
    void createFolder(const QString& sPath);
    void openDialog();
    void removeTab(int index);
    void copyPath(const QString& sPath);
    void searchFile(const QString& sPath);
    void searchInFolder(const QString& sPath);
    void revealInFinder(const QString& sPath);
    void closeTabs();

protected:
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
