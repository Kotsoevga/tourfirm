#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "ui_client.h"

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

    void setDB(QSqlDatabase tours,  QSqlQuery *queryTours, QSqlDatabase clients,  QSqlQuery *queryClients, QSqlDatabase reservations,  QSqlQuery *queryReservations){
        tours_ = tours;
        queryTours_ = queryTours;
        clients_ = clients;
        queryClients_ = queryClients;
        reservations_ = reservations;
        queryReservations_ = queryReservations;
    };

    void setStyle(const QString& background, const QString& textColor, QString buttonColor){
        setStyleSheet(background + textColor);
        ui->delete_2->setStyleSheet(textColor + buttonColor);
        ui->save->setStyleSheet(textColor + buttonColor);
        ui->reload_table->setStyleSheet(textColor + buttonColor);
        ui->update->setStyleSheet(textColor + buttonColor);
    }


private slots:

    void on_save_clicked();

    void on_update_clicked();

    void on_reload_table_clicked();

    void on_delete_2_clicked();

private:
    Ui::client *ui;

    QSqlDatabase tours_;
    QSqlQuery *queryTours_;

    QSqlDatabase clients_;
    QSqlQuery *queryClients_;

    QSqlDatabase reservations_;
    QSqlQuery *queryReservations_;
};

#endif // CLIENT_H
