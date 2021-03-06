#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name= "";
    arduino = new QSerialPort;


    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo,QSerialPortInfo::availablePorts()){
        qDebug() << "Has Vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }

        }

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
            if(serialPortInfo.vendorIdentifier()==arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }

            }
    if(arduino_is_available){
        //open and configure the port
        arduino -> setPortName(arduino_port_name);
        arduino -> open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino ->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }
    else{
        QMessageBox::warning(this, "Port Error", "Couldn't find the Arduino!");
    }

}

MainWindow::~MainWindow()
{
    if (arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}



void MainWindow::on_begin_clicked()
{
    MainWindow::updateGUI(QString("l"));

}

void MainWindow::on_end_clicked()
{
    MainWindow::updateGUI(QString("r"));

}

void MainWindow::updateGUI(QString command){
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());

    }
    else{
        qDebug() << " Couldn't write to serial.";
    }
}

void MainWindow::readSerial()
{

   QByteArray grams = arduino->readAll();
   qDebug() << grams ;
   QString parsedForce_data = grams;
   MainWindow::updateForce(parsedForce_data);
   MainWindow::on_recordData_clicked(parsedForce_data);


}


void MainWindow::updateForce(QString sensor_reading)
{
    ui->forceNumber->display(sensor_reading);
 }


void MainWindow::on_recordData_clicked(QString feltForce)
{
    if(ui->yesForce->isChecked())
    {
        QString filename="C:/Users/Jamie Kunnappally/Documents/Spring 2017 WOOHOO/Senior Design/ForceTest.txt";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << "Force Felt" + ui->fingerSelect->currentText() + feltForce << endl;
        }
        else if(ui->noForce->isChecked())
        {
            QString filename="C:/Users/Jamie Kunnappally/Documents/Spring 2017 WOOHOO/Senior Design/NoForceTest.txt";
            QFile file( filename );
            if ( file.open(QIODevice::ReadWrite) )
            {
                QTextStream stream( &file );
                stream << "No Force Felt" << endl;
            }
    }
}

}
