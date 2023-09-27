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
        //QVariantList data = database->executeQuery(QString("SELECT %1 FROM users WHERE ID = %2").arg(database->getColumnName(index.column())).arg(index.row()));

        SingleTableDatabase::QueryExecutionResult result = database->executeQuery(QString("SELECT %1 FROM (SELECT id, first_name, last_name, email, gender, row_number()").arg(database->getColumnName(index.column())) +
                                                   QString(" OVER (order by id) AS row_num FROM users) t WHERE row_num = %1").arg(index.row() + 1));

        if(result.queryExecutionResult == true && result.data.isEmpty() == false) return result.data.first();
    }

    return QVariant();
}
