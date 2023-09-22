#pragma once

#include <QAbstractTableModel>

class SingleTableDatabase;

class TableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    SingleTableDatabase* database {nullptr};
};
