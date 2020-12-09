//
// Created by Artem Shemidko on 08.12.2020.
//

#ifndef UTEXT_WINDOWMANAGER_H
#define UTEXT_WINDOWMANAGER_H

#include <QObject>
#include <mainwindow.h>



class WindowManager : public QObject {
Q_OBJECT

public slots:
    void addWindow(const QString &sPath);

//signals:
//
//    void grabFocus(QWidget *widget);

private:
   QWidget *m_lastFocus{Q_NULLPTR};
};


#endif //UTEXT_WINDOWMANAGER_H
