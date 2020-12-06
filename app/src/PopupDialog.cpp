//
// Created by Artem Shemidko on 05.12.2020.
//

#include "ui_popup.h"
#include "mainwindow.h"
#include "PopupDialog.h"
#include "FileObserver.h"
#include <qdebug.h>


PopupDialog::PopupDialog(const QString &msg, const Type& type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupDialog),
    type(type) {
    ui->setupUi(this);
    setFixedSize(std::max(parent->window()->width() / 4, 300), 85);
    ui->msg->setText(msg);
    exec();
}

PopupDialog::~PopupDialog() {
    delete ui;
}

void PopupDialog::accept() {
    auto entry = ui->entry->text();
    auto browser = static_cast<FileBrowser *>(parent());

    switch (type) {
        case Type::NewFile:
            browser->CreateFileCallback(entry);
            break;
        case Type::NewDir:
            browser->CreateFolderCallback(entry);
            break;
        case Type::SearchFile:
            browser->SearchFileCallback(entry);
            break;
        case Type::SearchInDir:
            browser->SearchInFolderCallback(entry);
            break;
    }
    QDialog::accept();
}



