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
    void on_begin_clicked(); //begin button
    void on_end_clicked(); //end button
    void updateGUI(QString); //creates the updateGUI function, sending the appropriate character to the Arduino through the serial port
    void updateForce(QString); //updates the LCD display
    void readSerial(); //reads in and parses force data from Arduino
    void on_recordData_clicked(QString); //saves data and exports as text file

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray serialData; //creates an array storing force readings
    QString serialBuffer; //creates a buffer for parsing data, delineated by commas
    QString parsed_data; //parsed data from force sensor
    double force_value; //the raw reading from force sensor
};

#endif // MAINWINDOW_H
