//
// Created by Artem Shemidko on 10.11.2020.
//

#include "ObserverModel.h"
#include <qdebug.h>
#include <QStandardItemModel>
#include <QApplication>
#include <QStyle>
#include <QDir>

ObserverModel::ObserverModel(QWidget *parent)
    : QFileSystemModel() {
}

QVariant ObserverModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if ((section == 0) && (role == Qt::DisplayRole)) {
        return "Project";
    } else {
        return QFileSystemModel::headerData(section, orientation, role);
    }
}

