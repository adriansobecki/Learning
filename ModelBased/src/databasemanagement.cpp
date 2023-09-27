#include "databasemanagement.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

SingleTableDatabase::SingleTableDatabase(QString databaseName, QString tableName): tableName(tableName)
{
    this->database = QSqlDatabase::addDatabase("QSQLITE");
    this->database.setDatabaseName(databaseName);

    if(this->database.open() == true)
    {
        QSqlQuery query;

        if(query.exec(QString("SELECT COUNT(*) FROM %1;").arg(tableName)))
        {
            if(query.next())
            {
                this->numberOfRows = query.value(0).toInt();
            }
        }

        if(query.exec(QString("PRAGMA table_info(\"%1\")").arg(tableName)))
        {
            while(query.next())
            {
                this->numberOfColumns = query.value(0).toInt();
                columnIndexToString[this->numberOfColumns] = query.value(1).toString();
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

        if(sqlQuery.exec(query))
        {
            result.queryExecutionResult = true;

            while(sqlQuery.next() == true)
            {
                QString row;

                for(int i = 0; i < sqlQuery.record().count(); i++)
                {
                    row += sqlQuery.value(i).toString();
                }

                result.data.append(row);
            }

            if(query.contains("INSERT", Qt::CaseInsensitive) || query.contains("DELETE", Qt::CaseInsensitive))
            {
                if(sqlQuery.exec(QString("SELECT COUNT(*) FROM %1;").arg(this->tableName)))
                {
                    if(sqlQuery.next())
                    {
                        this->numberOfRows = sqlQuery.value(0).toInt();
                    }
                }
            }
        }
    }

    return result;
}
