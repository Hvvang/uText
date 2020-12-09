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
    explicit MainWindow(const QString &sPath, QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void addContextMenuForBrowser(const QPoint &pos);
    void newFileCallback(const QString& sPath);
    void newFolderCallback(const QString& sPath);
    [[maybe_unused]] void replaceFileInView(const QString &sPath);
    void addFileToView(const QString &sPath);

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
    void newWindowCallback(const QString& sPath);


public slots:
    void renameFile(const QString &oldPath, const QString &newPath);


protected:
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
