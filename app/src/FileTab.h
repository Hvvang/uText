//
// Created by Artem Shemidko on 04.12.2020.
//

#ifndef UTEXT_FILETAB_H
#define UTEXT_FILETAB_H

#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QEvent>
#include <QDrag>
#include <QDropEvent>
#include <QFormLayout>
#include <QSplitter>
#include <QTabWidget>
//#include "tabchanger.h"
//#include "textArea.h"
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FileTab : public QTabWidget {
Q_OBJECT
private:
    std::map<QString, QTextEdit *> tab_content;

public:
    explicit FileTab(QWidget* parent = nullptr);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void addFile(QString filePath, Ui::MainWindow *ui);

//    TextArea *getTextArea();
//    TextArea *getTextArea(const QString& filename);
//    QString getFilename(int index);

public slots:
    void closeTab(int index);
    void updateTabName();
    void renameFile(QString oldPath, QString newPath);
    void removeFile(QString filePath);
};


#endif //UTEXT_FILETAB_H
