#include "databasemanagement.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

SingleTableDatabase::SingleTableDatabase(QString database, QString table): databaseName(database), tableName(table)
{
    this->database = QSqlDatabase::addDatabase("QSQLITE");
    this->database.setDatabaseName(this->databaseName);

    if(this->database.open() == true)
    {
        QSqlQuery query;

        if(query.exec(QString("SELECT COUNT(*) FROM %1;").arg(this->tableName)))
        {
            if(query.next() == true)
            {
                this->numberOfRows = query.value(0).toInt();
            }
        }

        if(query.exec(QString("PRAGMA table_info(\"%1\")").arg(this->tableName)))
        {
            while(query.next() == true)
            {
                this->numberOfColumns = query.value(0).toInt() + 1;

                this->columnIndexToString.append(query.value(1).toString());
            }
        }
    }
}

SingleTableDatabase::~SingleTableDatabase()
{
    this->database.close();
}

SingleTableDatabase::QueryExecutionResult SingleTableDatabase::executeQuery(QString query)
{
    SingleTableDatabase::QueryExecutionResult result;

    if(this->database.open() == true)
    {
        QSqlQuery sqlQuery;

        if(sqlQuery.exec(query) == true)
        {
            result.queryExecutionResult = true;

            while(sqlQuery.next() == true)
            {
                QString rowData;

                for(int i = 0; i < sqlQuery.record().count(); i++)
                {
                    rowData += sqlQuery.value(i).toString();
                }

                result.data.append(rowData);
            }

            if(query.contains("INSERT", Qt::CaseInsensitive) || query.contains("DELETE", Qt::CaseInsensitive))
            {
                if(sqlQuery.exec(QString("SELECT COUNT(*) FROM %1;").arg(this->tableName)) == true)
                {
                    if(sqlQuery.next() == true)
                    {
                        this->numberOfRows = sqlQuery.value(0).toInt();
                    }
                }
            }
        }
    }

    return result;
}
