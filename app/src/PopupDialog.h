//
// Created by Artem Shemidko on 05.12.2020.
//

#ifndef UTEXT_POPUPDIALOG_H
#define UTEXT_POPUPDIALOG_H

#include <QDialog>
#include <QEvent>
#include "FileBrowser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PopupDialog; }
QT_END_NAMESPACE

enum class Type {
    NewFile,
    NewDir,
    SearchFile,
    SearchInDir,
    Rename,
};

class PopupDialog : public QDialog {
Q_OBJECT

public:
    PopupDialog(QWidget *parent = nullptr);
    ~PopupDialog();

    void accept() override;
    void setParams(const QString& msg, const QString& defaultEntry, const Type& type);

signals:
    void NewFile(const QString& file);
    void NewFolder(const QString& file);
    void SearchFile(const QString& file);
    void SearchInDir(const QString& data);
    void Rename(const QString& newName);

private:
    Ui::PopupDialog *ui;
    Type type;
};


#endif //UTEXT_POPUPDIALOG_H
