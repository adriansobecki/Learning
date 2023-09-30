#pragma once
#include <QtSql/QSqlDatabase>
#include <map>
#include <QVariantList>

class SingleTableDatabase
{
public:
    struct QueryExecutionResult
    {
        bool queryExecutionResult { false };
        QVariantList data;
    };

    ~SingleTableDatabase();

    SingleTableDatabase::QueryExecutionResult executeQuery(QString query);

    QString getColumnName(int index) const { return columnIndexToString.at(index); };

    int getNumberOfRows() const { return numberOfRows; };
    int getNumberOfColumns() const { return numberOfColumns; };

private:
    SingleTableDatabase(QString database, QString table);
    SingleTableDatabase(const SingleTableDatabase&) = delete;
    SingleTableDatabase& operator=(const SingleTableDatabase&) = delete;

    QSqlDatabase database;

    QList<QString> columnIndexToString;

    QString tableName, databaseName;

    int numberOfRows { 0 };
    int numberOfColumns { 0 };

    friend class DatabasesManagement;
};

class DatabasesManagement
{
public:
    static DatabasesManagement& getInstance()
    {
        static DatabasesManagement instance;
        return instance;
    }

    SingleTableDatabase* getDatabase(QString databaseName, QString tableName)
    {
        if(this->databases.count(databaseName) == 0)
        {
            this->databases[databaseName] = std::unique_ptr<SingleTableDatabase>(new SingleTableDatabase(databaseName, tableName));
        }
        return this->databases.at(databaseName).get();
    }

private:
    std::map<QString, std::unique_ptr<SingleTableDatabase>> databases;
};
