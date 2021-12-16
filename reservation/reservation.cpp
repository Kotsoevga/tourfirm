#include "reservation.h"
#include "ui_reservation.h"
#include <reservationsFunctions.h>
#include <QMessageBox>
#include <QSqlQueryModel>

reservation::reservation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::reservation)
{
    ui->setupUi(this);
}


reservation::~reservation()
{
    delete ui;
}

void reservation::on_save_clicked()
{
    QString id, id_hotel, name_hotel, id_client, name_client, dates;
    id = ui->line_id->text();
    id_hotel =ui->line_id_tour->text(); //id тура/отеля
    id_client = ui ->line_id_client -> text(); //id клиента который забронировал
    dates = ui->line_dates->text(); //даты забронированного тура

    if(reservations_.open()){
       qDebug()<<"reservations opened in reservation window!";
    } else{
       qDebug()<<"reservations NOT opened in reservation window!";
    }

    switch(checkDataReservation(id, id_hotel, id_client, dates, queryReservations_, queryTours_, queryClients_, 1)){
    case 1:
        QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
        break;
    case 2:
        QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
        break;
    case 3: QMessageBox::critical(this, tr("Error"), tr("Such id alredy exists"));
        break;
    case 4: QMessageBox::critical(this, tr("Error"), tr("Invalid data of id_hotel"));
        break;
    case 5: QMessageBox::critical(this, tr("Error"), tr("Such hotel_id doesn't exist"));
        break;
    case 6: QMessageBox::critical(this, tr("Error"), tr("Invalid data of id_client"));
        break;
    case 7: QMessageBox::critical(this, tr("Error"), tr("Such client_id doesn't exist"));
        break;
    case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid dates format! Valid format: XX.YY.ZZZZ-XX.YY.ZZZZ"));
        break;
    case 9: QMessageBox::critical(this, tr("Error"), tr("there are no available rooms in this tour"));
        break;
    default:
    //при добавлениии добавить +stars current_tour, уменьшить available rooms в турах

        name_client = getName(id_client, queryClients_, 2);
        name_hotel = getName(id_hotel, queryTours_, 1);
        QString price_per_day = getPrice (id_hotel, queryTours_, queryMultipliers_);
        QString price = calculcatePrice(dates, price_per_day);
        queryReservations_->prepare("insert into Reservations (id, id_hotel, name_hotel, id_client, name_client, dates, total_price) values('"+id+"', '"+id_hotel+"', '"+name_hotel+"', '"+id_client+"', '"+name_client+"', '"+dates+"', '"+price+"')");

        if(queryReservations_->exec()){
            QMessageBox::information(this, tr("Saved status"), tr("Reservation data saved"));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("data not saved"));
        }
        updateClients_whenAddReservation(id_client, id, queryClients_); //устанавливаем current_tour и увеличиваем score на 1
        updateTours_whenAddReservation(id_hotel, queryTours_); //уменьшаем available_rooms на 1
        break;
    }


}


void reservation::on_update_clicked()
{
    QString id, id_hotel, name_hotel, id_client, name_client, dates;
    id = ui->line_id->text();
    id_hotel =ui->line_id_tour->text(); //id тура/отеля
    id_client = ui ->line_id_client -> text(); //id клиента который забронировал
    dates = ui->line_dates->text(); //даты забронированного тура
    if(reservations_.open()){
       qDebug()<<"reservations opened in reservation window!";
    } else{
       qDebug()<<"reservations NOT opened in reservation window!";
    }

    switch(checkDataReservation(id, id_hotel, id_client, dates, queryReservations_, queryTours_, queryClients_, 2)){
    case 2:
        QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
        break;
    case 4: QMessageBox::critical(this, tr("Error"), tr("Invalid data of id_hotel"));
        break;
    case 5: QMessageBox::critical(this, tr("Error"), tr("Such hotel_id doesn't exist"));
        break;
    case 6: QMessageBox::critical(this, tr("Error"), tr("Invalid data of id_client"));
        break;
    case 7: QMessageBox::critical(this, tr("Error"), tr("Such client_id doesn't exist"));
        break;
    case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid dates format! Valid format: XX.YY.ZZZZ-XX.YY.ZZZZ"));
        break;
    case 9: QMessageBox::critical(this, tr("Error"), tr("there are no available rooms in this tour"));
        break;
    case 10: QMessageBox::critical(this, tr("Error"), tr("fields must be filled"));
        break;
    case 11: QMessageBox::critical(this, tr("Error"), tr("Such id doesn't exist"));
        break;
    default:
        //при изменении tour_id  увеличиваем available_rooms на 1 в старом и уменьшаем на 1 в новом
        //при изменении id_client убираем currentTour_id у клиента и уменьшаем score у старого клиента и добавляем это у нового
        //при именении dates просто меняем дату

        if (!id_hotel.isEmpty()){
            QString old_id_hotel = getIdTour(id, queryReservations_); //получаем старое значение тура, который надо будет изменить
            updateTours_whenUpdateReservation(old_id_hotel, id_hotel, queryTours_);

            //УСТАНОВИТЬ NAME ОТЕЛЯ
            name_hotel = getName(id_hotel, queryTours_, 1);
            QString price_per_day = getPrice (id_hotel, queryTours_, queryMultipliers_);
            QString price = calculcatePrice(dates, price_per_day);

            queryReservations_->exec("UPDATE Reservations SET id_hotel = '"+id_hotel+"', name_hotel = '"+name_hotel+"', total_price = '"+price+"' where id ='"+id+"'");
        }

       if (!id_client.isEmpty()){
       QString old_id_client = getIdClient(id, queryReservations_);
       updateClients_whenUpdateReservation(old_id_client, id_client, id, queryClients_);

       //УСТАНОВИТЬ ИМЯ КЛИЕНТА +
       //ДОБАВИТЬ В ТАБЛИЦУ ЦЕНУ ЗАБРОНИРОВАННОГО ТУРА +


        name_client = getName(id_client, queryClients_, 2);
        queryReservations_->exec("UPDATE Reservations SET id_client = '"+id_client+"', name_client = '"+name_client+"' where id ='"+id+"'");
       }

       if (!dates.isEmpty()){
           id_hotel = getIdHotel(id, queryReservations_);
           QString price_per_day = getPrice (id_hotel, queryTours_, queryMultipliers_);
           QString price = calculcatePrice(dates, price_per_day);

            queryReservations_->exec("UPDATE Reservations SET dates = '"+dates+"', total_price = '"+price+"' where id ='"+id+"'");
       }
       QMessageBox::information(this, tr("Update status"), tr("Successfully updated"));

    }

}


void reservation::on_delete_2_clicked()
{
    QString id, id_client, id_hotel;
    id = ui->line_id->text();
    if ((id.toUInt()) && (already_exist(id, queryReservations_, 3)) && (!id.isEmpty())){ //если id введен корректно

        id_client = getIdClient(id, queryReservations_);
        id_hotel = getIdTour(id, queryReservations_);

        //у отеля available_rooms+1;

        updateClient_whenDeleteReservation(id_client, id, queryClients_);

        updateTour_whenDeleteReservation(id_hotel, queryTours_);
        queryReservations_->prepare("delete from Reservations where id = '"+id+"'");
        if(queryReservations_->exec()){
            QMessageBox::information(this, tr("Delete status"), tr("Successfully deleted"));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Data not deleted"));
        }


    } else{
        QMessageBox::critical(this, tr("Error"), tr("data not updated"));
    }

}


void reservation::on_reload_clicked()
{
    QSqlQueryModel* modelTours = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryReservations_;

    selectQuery -> prepare("SELECT * from Reservations");
    selectQuery->exec();

    modelTours ->setQuery(*selectQuery);
    ui->tableView->setModel(modelTours);
}

