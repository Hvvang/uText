//
// Created by Artem Shemidko on 08.12.2020.
//

#include "WindowManager.h"


void WindowManager::addWindow(const QString &sPath) {
    auto w = new MainWindow(sPath);
    connect(w, &MainWindow::newWindowCallback, this, &WindowManager::addWindow);
    m_lastFocus = w;
    w->show();
}

//void WindowManager::grabFocus(QWidget *widget) {
//    m_lastFocus = widget;
//}
