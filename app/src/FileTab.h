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
#include <QTextEdit>


class FileTab : public QTabWidget {
Q_OBJECT

public:
    explicit FileTab(QWidget* parent = nullptr);
    bool event(QEvent *event) override;
    void addFile(const QString &label, QTextEdit *widget);
    const QString &Path(const QString &label) const;

signals:
    void grabFocus(QWidget *widget);
    void closePanel();

public slots:
    void removeFileCallback(int index);
    void saveFileCallback(int index);
    void saveFileAsCallback(QString &file);
    void TabAboutToClose();
    void TabAboutToSave();
    void TabAboutToSaveAs();
    void TabAboutToSaveAll();
    void TabAboutToRename(const QString &oldPath, const QString &newPath);

private:
    QMap<QString, QTextEdit *> tabs;
};


#endif //UTEXT_FILETAB_H
