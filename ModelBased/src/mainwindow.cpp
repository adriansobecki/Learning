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

    database->executeQuery("INSERT INTO Users (ID, FirstName, LastName) VALUES (1, 'John', 'Doe');");
}

void MainWindow::on_deleteRowButton_clicked()
{
    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    database->executeQuery("DELETE FROM users WHERE ID = (SELECT MIN(ID) FROM users);");
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

    this->start = std::chrono::system_clock::now();

    auto database = DatabasesManagement::getInstance().getDatabase("database.db", "users");

    QVariantList data = database->executeQuery(QString("SELECT * FROM (SELECT id, first_name, last_name, email, gender, row_number()") +
                                               QString(" OVER (order by id) AS row_num FROM users) t"));

    for(const QVariant& row : data)
    {
        ui->listWidget->addItem(row.toString());
    }

    this->end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    ui->timeLabel->setText(QString::number(elapsed_seconds.count()));
}