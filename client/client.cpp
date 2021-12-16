#include "client.h"
#include "ui_client.h"
#include <QMessageBox>
#include <clientFunctions.h>
#include <QSqlQueryModel>

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
}

client::~client()
{
    delete ui;
}



void client::on_save_clicked()
{
    QString id, name, surname, birthday, tours_score;
    id = ui->line_id->text();
    name =ui->line_name->text(); //имя клиента
    surname = ui ->line_surname -> text(); //фамилия клиента
    birthday = ui->line_birthday->text(); //дата рождения клиента
    tours_score = ui->line_score->text(); //количество очков (забронированных туров за все время) для рейтинга



    if(clients_.open()){
       qDebug()<<"clients opened in client window!";
    } else{
       qDebug()<<"clients NOT opened in client window!";
    }

   switch(checkDataClient(id, name, surname, birthday, tours_score, queryClients_, 1)){ //проверяем корректность введеных данных
   case 1:
       QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 3: QMessageBox::critical(this, tr("Error"), tr("Such id alredy exists"));
       break;
   case 4: QMessageBox::critical(this, tr("Error"), tr("Invalid format of date! Valid format: XX.YY.ZZZZ"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of score"));
       break;
   default: //queryClients_->prepare("insert into Clients (id, name, surname, birthday, current_tour_id, tours_score) values('"+id+"', '"+name+"', '"+surname+"', '"+birthday+"', '"+current_tour_id+"', '"+tours_score+"'");
       queryClients_->prepare("insert into Clients (id, name, surname, birthday, current_tour_id, tours_score) values('"+id+"', '"+name+"', '"+surname+"', '"+birthday+"', 0, '"+tours_score+"')");

       if(queryClients_->exec()){
           QMessageBox::information(this, tr("Saved status"), tr("Data saved"));
       } else {
           QMessageBox::critical(this, tr("Error"), tr("data not saved"));
       }
       break;
    }

}

//ПРИ ИЗМЕНЕНИИ ИМЕНИ СДЕЛАТЬ ИЗМЕНЕНИЕ В БАЗЕ ДАННЫХ БРОНИРОВАНИЯ NAME CLIENT +

void client::on_update_clicked()
{
    QString id, name, surname, birthday, tours_score;
    id = ui->line_id->text();
    name =ui->line_name->text(); //имя клиента
    surname = ui ->line_surname -> text(); //фамилия клиента
    birthday = ui->line_birthday->text(); //дата рождения клиента
    tours_score = ui->line_score->text(); //количество очков (забронированных туров за все время) для рейтинга

    if(clients_.open()){
       qDebug()<<"clients opened in tour window!";
    } else{
       qDebug()<<"clients NOT opened in tour window!";
    }

   switch(checkDataClient(id, name, surname, birthday, tours_score, queryClients_, 2)){ //проверяем корректность введеных данных

   case 7:
       QMessageBox::critical(this, tr("Error"), tr("Fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 6: QMessageBox::critical(this, tr("Error"), tr("Such id doesn't exists"));
       break;
   case 4: QMessageBox::critical(this, tr("Error"), tr("Invalid format of date! Valid format: XX.YY.ZZZZ"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of score"));
       break;
   case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid symbols in name"));
       break;
   case 9: QMessageBox::critical(this, tr("Error"), tr("Invalid symbols in surname"));
       break;

   default:
       if (!name.isEmpty()){
           updateReservations(id, name, queryReservations_); //изменяем name_client в базе данных бронирования
           queryClients_->prepare("UPDATE Clients SET name = '"+name+"' where id ='"+id+"'");
           if (queryClients_->exec()){
           QMessageBox::information(this, tr("Update status"), tr("name updated"));
           } else{
                QMessageBox::critical(this, tr("Error"), tr("data not updated"));
           }
       }
       if (!surname.isEmpty()){
           queryClients_->prepare("UPDATE Clients SET surname = '"+surname+"' where id ='"+id+"'");
           if (queryClients_->exec()){
           QMessageBox::information(this, tr("Update status"), tr("surname updated"));
           } else{
                QMessageBox::critical(this, tr("Error"), tr("data not updated"));
           }
       }
       if (!birthday.isEmpty()){
           queryClients_->prepare("UPDATE Clients SET birthday = '"+birthday+"' where id ='"+id+"'");
           if (queryClients_->exec()){
           QMessageBox::information(this, tr("Update status"), tr("birthday updated"));
           } else{
                QMessageBox::critical(this, tr("Error"), tr("data not updated"));
           }
       }
       if (!tours_score.isEmpty()){
           queryClients_->prepare("UPDATE Clients SET tours_score = '"+tours_score+"' where id ='"+id+"'");
           if (queryClients_->exec()){
           QMessageBox::information(this, tr("Update status"), tr("Score updated"));
           } else {
                QMessageBox::critical(this, tr("Error"), tr("data not updated"));

           }
       }


   }
}


void client::on_reload_table_clicked()
{
    QSqlQueryModel* modelTours = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryClients_;

    selectQuery -> prepare("SELECT * from Clients");
    selectQuery->exec();

    modelTours ->setQuery(*selectQuery);
    ui->tableView->setModel(modelTours);
}



//При удалении удалить также в бронировании где этот клиент и available rooms увеличить в туре
void client::on_delete_2_clicked()
{
    QString id;
    id = ui->line_id->text();
    if(clients_.open()){
       qDebug()<<"clients opened in tour window!";
    } else{
       qDebug()<<"clients NOT opened in tour window!";
    }

    if (!id_exists(id, queryClients_) || (id.isEmpty())){
         QMessageBox::critical(this, tr("Error"), tr("id doesn't exist"));
    } else {

       updateToursReservations(id, queryReservations_, queryTours_);
       queryClients_->prepare("delete from Clients where id = '"+id+"'");

       if(queryClients_->exec()){
           QMessageBox::information(this, tr("Delete status"), tr("Successfully deleted"));
       } else {
           QMessageBox::critical(this, tr("Error"), tr("Data not deleted"));
       }
    }
}
