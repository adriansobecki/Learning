#include <QTableView>
#include <QListView>
#include <QtSql/QSqlQuery>
#include <QLabel>
#include "databasemanagement.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DatabasesManagement::getInstance().getDatabase("database.db", "users");

    ui->listView->setHidden(true);
    ui->tableView->setHidden(true);
    ui->sqlTableView->setHidden(true);
    ui->listWidget->setHidden(true);

    this->on_sqlTableViewButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addRowButton_clicked()
{
    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    SingleTableDatabase::QueryExecutionResult result = database->executeQuery("SELECT MAX(ID) FROM users;");

    database->executeQuery(QString("INSERT INTO Users (id, first_name, last_name, email, gender) VALUES (%1, 'first', 'last', 'email', 'gender');").arg(result.data[0].toInt() + 1));
}

void MainWindow::on_deleteRowButton_clicked()
{
    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    SingleTableDatabase::QueryExecutionResult result = database->executeQuery("SELECT id FROM users ORDER BY id;");

    database->executeQuery(QString("DELETE FROM users WHERE id IN (%1, %2);").arg(result.data[0].toString()).arg(result.data[2].toString()));
}

void MainWindow::on_listModelAddRowButton_clicked()
{
    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    SingleTableDatabase::QueryExecutionResult result = database->executeQuery("SELECT MAX(ID) FROM users;");

    if(listModel == nullptr) listModel = new ListModel(nullptr);

    listModel->addRow({QString::number(result.data[0].toInt() + 1), "first", "second", "email", "gender"});
}

void MainWindow::on_listModelDeleteRowButton_clicked()
{
    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    if(listModel == nullptr) listModel = new ListModel(nullptr);

    listModel->deleteRows({0, 2});
}

void MainWindow::on_sqlTableViewButton_clicked()
{
    ui->listView->setHidden(true);
    ui->tableView->setHidden(true);
    ui->sqlTableView->setHidden(false);
    ui->listWidget->setHidden(true);

    if(sqlTableModel) delete sqlTableModel;

    this->start = std::chrono::system_clock::now();

    DatabasesManagement::getInstance().getDatabase("database.db", "users");

    this->sqlTableModel = new QSqlTableModel(this);

    this->sqlTableModel->setTable("users");

    this->sqlTableModel->select();

    ui->sqlTableView->setModel(sqlTableModel);

    this->end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    ui->timeLabel->setText(QString::number(elapsed_seconds.count()));

    //ui->sqlTableView->hideColumn(2);
}

void MainWindow::on_listViewButton_clicked()
{
    ui->listView->setHidden(false);
    ui->tableView->setHidden(true);
    ui->sqlTableView->setHidden(true);
    ui->listWidget->setHidden(true);

    if(listModel) delete listModel;

    this->start = std::chrono::system_clock::now();

    this->listModel = new ListModel(nullptr);
    ui->listView->setModel(listModel);

    this->end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    ui->timeLabel->setText(QString::number(elapsed_seconds.count()));
}

void MainWindow::on_tableViewButton_clicked()
{
    ui->listView->setHidden(true);
    ui->tableView->setHidden(false);
    ui->sqlTableView->setHidden(true);
    ui->listWidget->setHidden(true);

    if(tableModel) delete tableModel;

    this->start = std::chrono::system_clock::now();

    this->tableModel = new TableModel(nullptr);

    ui->tableView->setModel(tableModel);

    this->end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    ui->timeLabel->setText(QString::number(elapsed_seconds.count()));
}

void MainWindow::on_withoutModelButton_clicked()
{
    ui->listView->setHidden(true);
    ui->tableView->setHidden(true);
    ui->sqlTableView->setHidden(true);
    ui->listWidget->setHidden(false);

    ui->listWidget->clear();

    this->start = std::chrono::system_clock::now();

    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    SingleTableDatabase::QueryExecutionResult result = database->executeQuery(QString("SELECT * FROM (SELECT id, first_name, last_name, email, gender, row_number()") +
                                               QString(" OVER (order by id) AS row_num FROM users) t"));

    if(result.queryExecutionResult == true)
    {
        for(const QVariant& row : result.data)
        {
            ui->listWidget->addItem(row.toString());
        }
    }

    this->end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    ui->timeLabel->setText(QString::number(elapsed_seconds.count()));
}