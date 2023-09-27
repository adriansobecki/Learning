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
        SingleTableDatabase::QueryExecutionResult result = database->executeQuery(QString("SELECT * FROM (SELECT id, first_name, last_name, email, gender, row_number()") +
                                                   QString(" OVER (order by id) AS row_num FROM users) t WHERE row_num = %1").arg(index.row() + 1));
        
        if(result.queryExecutionResult == true && result.data.isEmpty() == false) return result.data.first();
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

void ListModel::addRow(QList<QString> data)
{
    QString columns, values;
    
    for(int columnNumber = 0; columnNumber < this->database->getNumberOfColumns(); columnNumber++)
    {
        if(columns.isEmpty() == true) columns = QString("'%1'").arg(database->getColumnName(columnNumber));
        else columns = QString("%1, '%2'").arg(columns).arg(database->getColumnName(columnNumber));

        if(values.isEmpty() == true) values = QString("'%1'").arg(data[columnNumber]);
        else values = QString("%1, '%2'").arg(values).arg(data[columnNumber]);
    }

    QString query = QString("INSERT INTO Users (%1) VALUES (%2);").arg(columns).arg(values);

    beginInsertRows(QModelIndex(), data[0].toInt(), data[0].toInt());

    database->executeQuery(query);

    endInsertRows();
}

void ListModel::deleteRows(QList<int> indexes)
{
    SingleTableDatabase::QueryExecutionResult result = database->executeQuery("SELECT id FROM users ORDER BY id;");

    QString ids;

    for(const int& index : indexes)
    {
        if(ids.isEmpty() == true) ids = QString("%2").arg(QString::number(result.data[index].toInt()));
        else ids = QString("%1, %2").arg(ids).arg(QString::number(result.data[index].toInt()));
    }

    QString query = QString("DELETE FROM users WHERE id IN (%1)").arg(ids);

    beginRemoveRows(QModelIndex(), indexes[0], indexes[indexes.size() - 1]);

    database->executeQuery(query);

    endRemoveRows();
}