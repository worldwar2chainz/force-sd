#include "patientlogin.h"
#include "ui_patientlogin.h"

patientLogin::patientLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::patientLogin)
{
    ui->setupUi(this);

    QSqlDatabase research = QSqlDatabase::addDatabase("QSQLITE");
    research.setDatabaseName("C:/SQLite3/research.db");

    if(!research.open())
        qDebug() << "Failed to open.";
    else
        qDebug() << "Connection to databse established.";

}

patientLogin::~patientLogin()
{
    delete ui;
}

void patientLogin::on_loginButton_clicked()
{
    QString patientName, date;
}
