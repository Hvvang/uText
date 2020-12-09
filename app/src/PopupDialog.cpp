//
// Created by Artem Shemidko on 05.12.2020.
//

#include "ui_popup.h"
#include "mainwindow.h"
#include "PopupDialog.h"
#include "FileObserver.h"
#include <qdebug.h>


PopupDialog::PopupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog) {
    ui->setupUi(this);
    setFixedSize(std::max(parent->window()->width() / 4, 300), 85);
}

PopupDialog::~PopupDialog() {
    delete ui;
}

void PopupDialog::setParams(const QString& msg, const QString& defaultEntry, const Type& type) {
    this->type = type;
    ui->msg->setText(msg);
    ui->entry->setText(defaultEntry);
    exec();
}

void PopupDialog::accept() {
    auto entry = ui->entry->text();

    switch (type) {
        case Type::NewFile:
            emit NewFile(entry);
            break;
        case Type::NewDir:
            emit NewFolder(entry);
            break;
        case Type::SearchFile:
            emit SearchFile(entry);
            break;
        case Type::SearchInDir:
            emit SearchInDir(entry);
            break;
        case Type::Rename:
            emit Rename(entry);
            break;
        case Type::Find:
            emit Find(entry);
            break;
        case Type::Replace:
            emit Replace(entry);
            break;
    }
    QDialog::accept();
}





