//
// Created by Artem Shemidko on 28.11.2020.
//

#ifndef UTEXT_TREEMODEL_H
#define UTEXT_TREEMODEL_H
#include <QAbstractItemModel>
#include "TreeItem.h"
#include <QModelIndex>
#include <QVariant>
#include <QFileSystemModel>

class TreeItem;

//! [0]
class TreeModel : public QAbstractItemModel
{
Q_OBJECT

public:
    TreeModel(const QStringList &headers, const QString &data,
              QObject *parent = nullptr);
    ~TreeModel();
//! [0] //! [1]

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//! [1]

//! [2]
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);
    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem;
    QFileSystemModel *root;
};


#endif //UTEXT_TREEMODEL_H
