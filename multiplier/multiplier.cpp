#include "multiplier.h"
#include "ui_multiplier.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <multipliersFunctions.h>

multiplier::multiplier(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::multiplier)
{
    ui->setupUi(this);
}

multiplier::~multiplier()
{
    delete ui;
}




void multiplier::on_reload_table_clicked()
{
    QSqlQueryModel* modelMultipliers = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryMultipliers_;

    selectQuery -> prepare("SELECT * from Multiplier");
    selectQuery->exec();

    modelMultipliers ->setQuery(*selectQuery);
    ui->tableView->setModel(modelMultipliers);
}


void multiplier::on_pushButton_clicked()
{
    QString percent, koef;
    percent = ui->line_percent->text();
    koef = ui->line_multiplier->text();
    switch(checkInputData(percent, koef, queryMultipliers_)){
    case 1:
        QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
        break;
    case 2:
        QMessageBox::critical(this, tr("Error"), tr("Invalid data of percent"));
        break;
    case 3:
        QMessageBox::critical(this, tr("Error"), tr("Invalid data of multiplier"));
        break;
    case 4:
        QMessageBox::critical(this, tr("Error"), tr("Multiplier must be more than 1"));
        break;
    case 5:
        QMessageBox::critical(this, tr("Error"), tr("Multipliers must increase"));
        break;
    case 6:
        QMessageBox::critical(this, tr("Error"), tr("Multiplier must be not more than 10"));
    default:
        queryMultipliers_->prepare("UPDATE Multiplier SET multiplier = '"+koef+"' where percent ='"+percent+"'");
        if (queryMultipliers_->exec()){
             QMessageBox::information(this, tr("Update status"), tr("multiplier updated"));
        } else{
             QMessageBox::critical(this, tr("Error"), tr("data not updated"));
        }
   }
}

