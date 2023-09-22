#include "listmodel.h"
#include "databasemanagement.h"
#include <QColor>
#include <QSize>

ListModel::ListModel(QObject *parent): QAbstractListModel(parent)
{
    database = DatabasesManagement::getInstance().getDatabase("database.db", "users");
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return database->getNumberOfRows();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        QVariantList data = database->executeQuery(QString("SELECT * FROM (SELECT id, first_name, last_name, email, gender, row_number()") +
                                                   QString(" OVER (order by id) AS row_num FROM users) t WHERE row_num = %1").arg(index.row() + 1));

        //QVariantList data = database->executeQuery(QString("SELECT * FROM users WHERE ID = %1").arg(index.row()));
        //qDebug() << data;

        if(data.isEmpty() == false) return data.first();
    }
    else if(role == Qt::BackgroundRole )
    {
        if (index.row() % 2 == 0) return QColor(Qt::lightGray);
        else return QColor(Qt::white);
    }
    else if(role == Qt::DecorationRole) return QColor(Qt::red);
    else if(role == Qt::SizeHintRole) return QSize(0, 50);
    else if(role == Qt::ForegroundRole) return QColor(Qt::blue);
    else if(role == Qt::CheckStateRole)
    {
        if(selectedRows.contains(index.row())) return Qt::Checked;
        else return Qt::Unchecked;
    }

    return QVariant();
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index)const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if(index.isValid() == true) return defaultFlags | Qt::ItemIsUserCheckable;
    else return defaultFlags;
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() == false) return false;

    if (role == Qt::CheckStateRole)
    {
        if (value == Qt::Checked) selectedRows.insert(index.row());
        else selectedRows.remove(index.row());

        emit dataChanged(index, index, {Qt::CheckStateRole});

        return true;
    }

    return false;
}
