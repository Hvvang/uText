//
// Created by Artem Shemidko on 09.12.2020.
//

#include "SaveDialog.h"
#include "ui_beforeclosedialog.h"

SaveDialog::SaveDialog(const QString &label, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SaveDialog) {
    ui->setupUi(this);
    if (!label.isEmpty()) {
        ui->Message->setText("'" + label + "' has changes, do you want to save them?");
    }
    connect(ui->unsaveBtn, &QPushButton::released, this, [=] { result = 0; QDialog::reject();});
    exec();
}

SaveDialog::~SaveDialog() {
    delete ui;
}

const int &SaveDialog::getResult() const {
    return result;
}

void SaveDialog::accept() {
    result = 2;
    QDialog::accept();
}

void SaveDialog::reject() {
    result = 1;
    QDialog::reject();
}
