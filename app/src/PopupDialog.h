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
};

class PopupDialog : public QDialog {
Q_OBJECT

public:
    PopupDialog(const QString& msg, const Type& type, QWidget *parent = nullptr);
    ~PopupDialog();

    void accept() override;

private:
    Ui::PopupDialog *ui;
    Type type;
};


#endif //UTEXT_POPUPDIALOG_H
