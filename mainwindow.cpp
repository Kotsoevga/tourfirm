#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPixmap"
#include <QMessageBox>
#include <QtWidgets>
#include "menu.h" //чтобы открыть второе окно
#include "msgwarning.h" //окно предупреждения о неверном пароле
#include <QDebug>

bool checkLogin (const QString& login, const QString& password, QSqlQuery* query){
    query->exec("SELECT * FROM Accounts");
    while (query->next()){
        QString bdLogin = query->value("Login").toString();
        QString bdPassword = query->value("Password").toString();
        if ((bdLogin == login) && (bdPassword == password)){
            return true;
        }
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pixmapPalm1( ":/files/leftImage" );
    QPixmap pixmapPalm2( ":/files/rightImage" );
    QPixmap pixmapPlane( ":/files/plane" );

    ui->leftPicture->setScaledPixmap(pixmapPalm1);
    ui->rightPicture->setScaledPixmap(pixmapPalm2);
    ui->labelPlane->setScaledPixmap(pixmapPlane);

    ui->labelText->setAlignment(Qt::AlignCenter);

    window = new menu();//инициализируем окно
    //подключаем к слоту запуска окна меню по кнопке во втором окне
    connect(window, &menu::openLoginWindow, this, &MainWindow::show);

    accounts = QSqlDatabase::addDatabase("QSQLITE", "connectionToAccounts");
    accounts.setDatabaseName("./accounts.db");
    if (accounts.open()){
        qDebug("open");
    }
    query = new QSqlQuery(accounts);

}

MainWindow::~MainWindow()
{
    delete window;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString login = ui->lineLogin->text();
    QString password = ui->linePassword->text();

     if (checkLogin(login, password, query)){ //проверяем есть ли такой логин и пароль в бд
         close();
         window->show();
         ui->lineLogin->clear();
         ui->linePassword->clear();
    } else {
        msgWarning msgWindow;
        msgWindow.exec();
    }
}
