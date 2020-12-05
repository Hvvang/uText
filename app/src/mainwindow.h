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

public:
    void menuConnector();
    void newCallback();
    void newFileCallback(const QString& sPath);
    void newDirCallback(const QString& sPath);
    void openCallback();
    void saveCallback();
    void saveAsCallback();
    void saveAllCallback();
    void closeCallback();

    void browserSwitch();

signals:
    void closeTabs();


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
