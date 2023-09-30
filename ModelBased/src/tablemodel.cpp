#include "tablemodel.h"
#include "databasemanagement.h"

TableModel::TableModel(QObject *parent): QAbstractTableModel(parent)
{
    database = DatabasesManagement::getInstance().getDatabase("database.db", "users");
}

int TableModel::rowCount(const QModelIndex&) const
{
   return database->getNumberOfRows();
}

int TableModel::columnCount(const QModelIndex&) const
{
    return database->getNumberOfColumns();
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() == false) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {   
        QString query("SELECT %1 FROM (SELECT row_number() OVER (order by id) AS row_num, * FROM users) t WHERE row_num = %2");
        
        query = query.arg(database->getColumnName(index.column())).arg(index.row() + 1);

        SingleTableDatabase::QueryExecutionResult result = database->executeQuery(query);

        if(result.queryExecutionResult == true && result.data.isEmpty() == false) return result.data.first();
    }

    return QVariant();
}
