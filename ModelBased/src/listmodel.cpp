#include "listmodel.h"
#include "databasemanagement.h"
#include <QColor>
#include <QSize>
#include <QString>
#include <QtSql/QSqlQuery>

ListModel::ListModel(QObject *parent): QAbstractListModel(parent)
{
    this->database = DatabasesManagement::getInstance().getDatabase("database.db", "users");
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return this->database->getNumberOfRows();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        QString query("SELECT * FROM (SELECT row_number() OVER (order by id) AS row_num, * FROM users) t WHERE row_num = %1");

        query = query.arg(index.row() + 1);

        SingleTableDatabase::QueryExecutionResult result = database->executeQuery(query);

        if(result.queryExecutionResult == true && result.data.isEmpty() == false) return result.data.first();
    }
    else if(role == Qt::BackgroundRole )
    {
        if (index.row() % 2 == 0) return QColor(Qt::lightGray);
        else return QColor(Qt::white);
    }
    else if(role == Qt::CheckStateRole)
    {
        if(selectedRows.contains(index.row())) return Qt::Checked;
        else return Qt::Unchecked;
    }
    else if(role == Qt::DecorationRole) return QColor(Qt::red);
    else if(role == Qt::SizeHintRole) return QSize(0, 50);
    else if(role == Qt::ForegroundRole) return QColor(Qt::blue);

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

void ListModel::addRow(QList<QString> data)
{
    if(data.size() != this->database->getNumberOfColumns() - 1) return;

    QString query("INSERT INTO users (first_name, last_name, email, gender) VALUES ('%1', '%2', '%3', '%4');");
    
    for(int i = 1; i < this->database->getNumberOfColumns(); i++)
    {
        query = query.arg(this->database->getColumnName(i));
    }

    beginInsertRows(QModelIndex(), database->getNumberOfRows(), database->getNumberOfRows());
    
    database->executeQuery(query);

    endInsertRows();
}

void ListModel::deleteRows(QList<int> indexes)
{
    SingleTableDatabase::QueryExecutionResult result = database->executeQuery("SELECT id FROM users ORDER BY id;");

    QString ids;

    for(const int& index : indexes)
    {
        if(ids.isEmpty() == true) ids = QString("%1").arg(QString::number(result.data[index].toInt()));
        else ids = QString("%1, %2").arg(ids).arg(QString::number(result.data[index].toInt()));
    }

    QString query = QString("DELETE FROM users WHERE id IN (%1)").arg(ids);

    auto [min, max] = std::minmax_element(indexes.begin(), indexes.end());

    beginRemoveRows(QModelIndex(), *min, *max);

    database->executeQuery(query);

    endRemoveRows();
}