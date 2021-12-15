#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QtWidgets>
#include "backgroundsetting.h"
#include <colorsetting.h>
#include <buttoncolor.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <tour.h>
#include <client.h>
#include <reservation.h>
#include <multiplier.h>

namespace Ui {
class menu;
}

class menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();

    void setBackground(QString background){
        currentBackground = background;
        setStyleSheet(currentBackground + currentTextColor);
    };

    void setColor(QString newTextColor);
    void setButtonColor(QString newButtonColor);


signals:
    void openLoginWindow();
    void giveBackground_to_colorsetting(QString backgroundColor);
    void giveBackground_to_buttoncolor(QString backgroundColor);
    void giveColor_to_buttoncolor(QString color);
    void giveButtonColor_to_colosetting (QString buttonColor);
    void giveColor_to_backgroundsetting(QString color);
    void giveDBinfo(QSqlDatabase& tours,  QSqlQuery *queryTours, QSqlDatabase& clients,  QSqlQuery *queryClients, QSqlDatabase& reservations,  QSqlQuery *queryReservations);
    void giveDBinfo2(QSqlDatabase& tours,  QSqlQuery *queryTours, QSqlDatabase& clients,  QSqlQuery *queryClients, QSqlDatabase& reservations,  QSqlQuery *queryReservations);
    void giveDBinfo3(QSqlDatabase& tours,  QSqlQuery *queryTours, QSqlDatabase& clients,  QSqlQuery *queryClients, QSqlDatabase& reservations,  QSqlQuery *queryReservations, QSqlDatabase& multipliers, QSqlQuery *queryMultipliers );
    void giveDBinfo4(QSqlDatabase& multiplier,  QSqlQuery *queryMultiplier);
    void giveStyleToTours(const QString& currentBackground, const QString& currentTextColor, const QString& currentButtonColor);
    void giveStyleToClients(const QString& currentBackground, const QString& currentTextColor, const QString& currentButtonColor);
    void giveStyleToReservations(const QString& currentBackground, const QString& currentTextColor, const QString& currentButtonColor);
    void giveStyleToMultiplier(const QString& currentBackground, const QString& currentTextColor, const QString& currentButtonColor);



private slots:
    void on_changeColorBackground_triggered();

    void on_exit_triggered();

    void on_changeColorSymbols_triggered();

    void on_action_triggered();

    void on_tours_clicked();

    void on_client_clicked();

    void on_booking_clicked();

    void on_coefficient_clicked();

private:
    Ui::menu *ui;

    backgroundSetting* windowBackground;
    colorSetting* windowColor;
    buttoncolor* windowButtonColor;
    tour* toursWindow;
    client* clientsWindow;
    reservation* reservationsWindow;
    multiplier* multiplierWindow;

    QString currentBackground;
    QString currentTextColor;
    QString currentButtonColor;

    QSqlDatabase tours;
    QSqlQuery *queryTours;

    QSqlDatabase clients;
    QSqlQuery *queryClients;

    QSqlDatabase reservations;
    QSqlQuery *queryReservations;

    QSqlDatabase multipliers;
    QSqlQuery *queryMultipliers;
};

#endif // MENU_H
