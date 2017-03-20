#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSerialPort>
#include <QWidget>
#include <QtSql>
#include <QFileInfo>
#include <QByteArray>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_begin_clicked();

    void on_end_clicked();
    void updateGUI(QString);
    void updateForce(QString);
    void readSerial();
    void on_recordData_clicked(QString);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray grams;
    QString serialBuffer;
    QString parsedForce_data;
    double force_value;
};

#endif // MAINWINDOW_H
