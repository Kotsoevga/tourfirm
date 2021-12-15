#ifndef RESERVATION_H
#define RESERVATION_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <ui_reservation.h>

namespace Ui {
class reservation;
}

class reservation : public QMainWindow
{
    Q_OBJECT

public:
    explicit reservation(QWidget *parent = nullptr);
    ~reservation();

    void setDB(QSqlDatabase tours,  QSqlQuery *queryTours, QSqlDatabase clients,  QSqlQuery *queryClients, QSqlDatabase reservations,  QSqlQuery *queryReservations, QSqlDatabase multipliers, QSqlQuery* queryMultipliers){
        tours_ = tours;
        queryTours_ = queryTours;
        clients_ = clients;
        queryClients_ = queryClients;
        reservations_ = reservations;
        queryReservations_ = queryReservations;
        multipliers_ = multipliers;
        queryMultipliers_ = queryMultipliers;
    };

    void setMyStyle(const QString& background, const QString& textColor, QString buttonColor){
    setStyleSheet(background + textColor);
    ui->delete_2->setStyleSheet(textColor + buttonColor);
    ui->save->setStyleSheet(textColor + buttonColor);
    ui->update->setStyleSheet(textColor + buttonColor);
    ui->reload->setStyleSheet(textColor + buttonColor);
    }

private slots:
    void on_save_clicked();

    void on_update_clicked();

    void on_delete_2_clicked();

    void on_reload_clicked();

private:
    Ui::reservation *ui;

    QSqlDatabase tours_;
    QSqlQuery *queryTours_;

    QSqlDatabase clients_;
    QSqlQuery *queryClients_;

    QSqlDatabase reservations_;
    QSqlQuery *queryReservations_;

    QSqlDatabase multipliers_;
    QSqlQuery *queryMultipliers_;

    bool boolScore;
};

#endif // RESERVATION_H
