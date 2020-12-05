//
// Created by Artem Shemidko on 05.12.2020.
//

#ifndef UTEXT_FILEBROWSER_H
#define UTEXT_FILEBROWSER_H

#include <QTabWidget>

class FileBrowser : public QTabWidget {
Q_OBJECT
public:
    explicit FileBrowser(QWidget* parent = nullptr);
    void addFolder(const QString &sPath);

public slots:
    void removeFolder(int index);
};


#endif //UTEXT_FILEBROWSER_H
