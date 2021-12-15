#ifndef TOUR_H
#define TOUR_H

#include <QMainWindow>
#include "ui_tour.h"
#include"QDebug"
#include "QMessageBox"
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>

namespace Ui {
class tour;
}

class tour : public QMainWindow
{
    Q_OBJECT

public:
    explicit tour(QWidget *parent = nullptr);
    ~tour();


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
        ui->deleteButton->setStyleSheet(textColor + buttonColor);
        ui->save_button->setStyleSheet(textColor + buttonColor);
        ui->reload_table->setStyleSheet(textColor + buttonColor);
        ui->update_button->setStyleSheet(textColor + buttonColor);
    }

private slots:

    void on_reload_table_clicked();

    void on_update_button_clicked();

    void on_save_button_clicked();

    void on_deleteButton_clicked();

private:
    Ui::tour *ui;

    QSqlDatabase tours_;
    QSqlQuery *queryTours_;

    QSqlDatabase clients_;
    QSqlQuery *queryClients_;

    QSqlDatabase reservations_;
    QSqlQuery *queryReservations_;

};

#endif // TOUR_H
