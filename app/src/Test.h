//
// Created by Artem Shemidko on 07.12.2020.
//

#ifndef UTEXT_TEST_H
#define UTEXT_TEST_H

#include <QWidget>
#include <QFile>
#include <QSplitter>
#include <QTabWidget>

class Test : public QWidget {
    Q_OBJECT
public:

    explicit Test(QWidget *parent = Q_NULLPTR);
    virtual ~Test();

    void setRootSplitter(QSplitter *rootSpliter);
    void splitUp();
    void splitDown();
    void splitLeft();
    void splitRight();
    void addPageToPanel(const QString& label, QFile *file);
public slots:
    void addNewWindow(QSplitter *root, const QPair<int, int> &pos);
    void LastFocusedTabController(QWidget *widget);

//    void dragMoveEvent(QDragMoveEvent *event) override;
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dropEvent(QDropEvent *event) override;
//protected:
//    void LastFocusedTabController(QWidget *widget);
//    void AddPageToLastFocus(QString label, QFile *file);
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
private:
//    void Split(const int x, const int y, QWidget *editor);
    QWidget *m_lastFocus{Q_NULLPTR};
    QSplitter *m_split{Q_NULLPTR};
    QSplitter *rootSplitter{Q_NULLPTR};

};


#endif //UTEXT_TEST_H
