//
// Created by Artem Shemidko on 10.11.2020.
//

#ifndef UTEXT_OBSERVERMODEL_H
#define UTEXT_OBSERVERMODEL_H

#include <QFileSystemModel>
#include <QAbstractItemModel>

class ObserverModel : public QFileSystemModel {
Q_OBJECT
public:
    explicit ObserverModel(QWidget *parent = Q_NULLPTR);

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

};


#endif //UTEXT_OBSERVERMODEL_H
