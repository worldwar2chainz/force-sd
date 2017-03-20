#ifndef PATIENTLOGIN_H
#define PATIENTLOGIN_H

#include <QDialog>
#include <QtSql>
#include <QFileInfo>
#include <QtDebug>

namespace Ui {
class patientLogin;
}

class patientLogin : public QDialog
{
    Q_OBJECT

public:
    explicit patientLogin(QWidget *parent = 0);
    ~patientLogin();

private slots:
    void on_loginButton_clicked();

private:
    Ui::patientLogin *ui;
    QSqlDatabase research;
};

#endif // PATIENTLOGIN_H
