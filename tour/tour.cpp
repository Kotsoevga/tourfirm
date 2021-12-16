#include "tour.h"
#include <tourFunctions.h>
#include <QSqlQueryModel>


tour::tour(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tour)
{
    ui->setupUi(this);
}

tour::~tour()
{
    delete ui;
}

void tour::on_save_button_clicked()
{
    QString id, name, availableRooms, amountRooms, price, stars, charter;
    id = ui->line_id->text();
    name =ui->line_name->text(); //название тура/отеля
    amountRooms = ui->line_all_rooms->text(); //всего свмест в отеле
    availableRooms = amountRooms;
    price = ui->line_price->text(); //цена
    stars = ui->line_stars->text(); //класс тура (количество звезд отеля)
    charter = ui->line_charter->text(); //иннформация чартер или нет
    if(tours_.open()){
       qDebug()<<"tours opened in tour window!";
    } else{
       qDebug()<<"tours NOT opened in tour window!";
    }

   switch(checkDataTour(id, name, availableRooms, amountRooms, price, stars, charter, queryTours_, 1)){ //проверяем корректность введеных данных
   case 1:
       QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 3: QMessageBox::critical(this, tr("Error"), tr("Such id alredy exists"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of amount of rooms"));
       break;
   case 7: QMessageBox::critical(this, tr("Error"), tr("Invalid price data"));
       break;
   case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid stars data"));
       break;
   case 9: QMessageBox::critical(this, tr("Error"), tr("The number of stars should be no more than 5"));
       break;
   case 10: QMessageBox::critical(this, tr("Error"), tr("Invalid charter data"));
       break;
   case 13: QMessageBox::critical(this, tr("Error"), tr("Invalid name symbols"));
       break;
   default: queryTours_->prepare("insert into Tours (id, name, available_rooms, amount_rooms, price, stars, charter) values('"+id+"', '"+name+"', '"+availableRooms+"', '"+amountRooms+"', '"+price+"', '"+stars+"', '"+charter+"')");
       if(queryTours_->exec()){
           QMessageBox::information(this, tr("Saved status"), tr("Data saved"));
       } else {
           QMessageBox::critical(this, tr("Error"), tr("Data not saved"));
       }
       break;
    }

}

//ПРИ ИЗМЕНЕНИИ ИМЕНИ СДЕЛАТЬ ИЗМЕНЕНИЕ В БАЗЕ ДАННЫХ БРОНИРОВАНИЯ NAME TOUR
void tour::on_update_button_clicked()
{
    QString id, name, availableRooms, amountRooms, price, stars, charter;
    id = ui->line_id->text();
    name =ui->line_name->text(); //название тура/отеля
    amountRooms = ui->line_all_rooms->text(); //всего мест в отеле
    availableRooms = amountRooms;
    price = ui->line_price->text(); //цена
    stars = ui->line_stars->text(); //класс тура (количество звезд отеля)
    charter = ui->line_charter->text(); //иннформация чартер или нет
    if(tours_.open()){
       qDebug()<<"tours opened in tour window!";
    } else{
       qDebug()<<"tours NOT opened in tour window!";
    }

   switch(checkDataTour(id, name, availableRooms, amountRooms, price, stars, charter, queryTours_, 2)){ //проверяем корректность введеных данных

   case 12:
       QMessageBox::critical(this, tr("Error"), tr("Fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 11: QMessageBox::critical(this, tr("Error"), tr("Such id doesn't exist"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of amount of rooms"));
       break;
   case 4: QMessageBox::critical(this, tr("Error"), tr("New value of amount of rooms is less than reservations"));
       break;
   case 7: QMessageBox::critical(this, tr("Error"), tr("Invalid price data"));
       break;
   case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid stars data"));
       break;
   case 9: QMessageBox::critical(this, tr("Error"), tr("The number of stars should be no more than 5"));
       break;
   case 10: QMessageBox::critical(this, tr("Error"), tr("Invalid charter data"));
       break;
   case 13:
       QMessageBox::critical(this, tr("Error"), tr("Invalid name symbols"));
              break;
   default:
       if (!name.isEmpty()){
            queryTours_->exec("UPDATE Tours SET name = '"+name+"' where id = '"+id+"'");
            updateNameInReservations(id, name, queryReservations_);
       }

       if (!amountRooms.isEmpty()){
       int newAvailableRoooms = amountRooms.toInt() - getAmountRooms(id, queryTours_).toInt() + getAvailableRooms(id,queryTours_).toInt(); //считаем новое значение количества доступных комнат
       std::string avRooms = std::to_string(newAvailableRoooms);
       availableRooms = makeQString(avRooms);
       queryTours_->exec("UPDATE Tours SET available_rooms = '"+availableRooms+"', amount_rooms = '"+amountRooms+"' where id = '"+id+"'");
       }

       if(!price.isEmpty()){
           queryTours_->exec("UPDATE Tours SET price ='"+price+"' where id = '"+id+"'");
       }

       if (!stars.isEmpty()){
           queryTours_->exec("UPDATE Tours SET stars ='"+stars+"' where id = '"+id+"'");
       }

       if (!charter.isEmpty()){
           queryTours_->exec("UPDATE Tours SET charter ='"+charter+"' where id = '"+id+"'");
       }
       QMessageBox::information(this, tr("Updated status"), tr("Data updated"));
   }
}


void tour::on_reload_table_clicked()
{

    QSqlQueryModel* modelTours = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryTours_;

    selectQuery -> prepare("SELECT * from Tours");
    selectQuery->exec();

    modelTours ->setQuery(*selectQuery);
    ui->tableView->setModel(modelTours);

}

void tour::on_deleteButton_clicked()
{
    QString id;
    id = ui->line_id->text();
    if(tours_.open()){
       qDebug()<<"tours opened in tour window!";
    } else{
       qDebug()<<"tours NOT opened in tour window!";
    }

    if (is_unique(id, queryTours_) || (id.isEmpty())){
         QMessageBox::critical(this, tr("Error"), tr("id doesn't exist"));
    } else {
       updateClientsReservations_whenDeleteTour(id, queryClients_, queryReservations_);
       queryTours_->prepare("delete from Tours where id = '"+id+"'");

       if(queryTours_->exec()){
           QMessageBox::information(this, tr("Delete status"), tr("Successfully deleted"));
       } else {
           QMessageBox::critical(this, tr("Error"), tr("Data not deleted"));
       }
    }
}

