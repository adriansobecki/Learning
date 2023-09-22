#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>

class SingleTableDatabase;

class ListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    ListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    SingleTableDatabase* database {nullptr};
    QSet<int> selectedRows;
};

#endif // LISTMODEL_H
