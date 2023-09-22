#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listmodel.h"
#include "tablemodel.h"
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addRowButton_clicked();

    void on_deleteRowButton_clicked();

    void on_sqlTableViewButton_clicked();

    void on_listViewButton_clicked();

    void on_tableViewButton_clicked();

    void on_withoutModelButton_clicked();

private:
    Ui::MainWindow *ui;
    ListModel* listModel { nullptr };
    TableModel* tableModel { nullptr };
    QSqlTableModel* sqlTableModel { nullptr };

    std::chrono::time_point<std::chrono::system_clock> start, end;
};
#endif // MAINWINDOW_H
