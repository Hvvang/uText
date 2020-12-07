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


class FileTab : public QTabWidget {
Q_OBJECT
private:
    std::map<QString, QTextEdit *> tab_content;
    QPair<int, int> pos;


public:
    explicit FileTab(QPair<int, int> pos, QWidget* parent = nullptr);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    [[nodiscard]] const QPair<int, int> &getPos() const;
    [[nodiscard]] const int &splitterPos() const;
    [[nodiscard]] const int &indexPos() const;

    void setPos(const QPair<int, int> &pos);
    bool event(QEvent *event) override;
//    void addFile(QString filePath, Ui::MainWindow *ui);

//    TextArea *getTextArea();
//    TextArea *getTextArea(const QString& filename);
//    QString getFilename(int index);

signals:
    void grabFocus(QWidget *widget);

public slots:
    void closeTab(int index);
    void updateTabName();
    void renameFile(QString oldPath, QString newPath);
    void removeFile(QString filePath);
};


#endif //UTEXT_FILETAB_H
