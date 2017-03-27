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
    arduino = new QSerialPort; //establishes new arduino connection
    serialBuffer = ""; //creates empty serial buffer QString
    parsed_data = " "; //creates empty parsed data QString
    force_value = 0.0; //initializes force value reading to 0


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
        arduino -> open(QSerialPort::ReadWrite); //allows us to read and write from serialport
        arduino->setBaudRate(QSerialPort::Baud9600); //set baud rate to 9600 (everyone's favorite baud rate)
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



void MainWindow::on_begin_clicked() //begin test function
{
    MainWindow::updateGUI(QString("f")); //sends 'f' to arduino
    MainWindow::readSerial(); //reads force sensor and updates LCD display

}

void MainWindow::on_end_clicked() //end test function
{
    MainWindow::updateGUI(QString("t")); //sends a 't' to the arduino

}

void MainWindow::updateGUI(QString command){
    if(arduino->isWritable()){ //basically if the serialport is open to receiving a message
        arduino->write(command.toStdString().c_str()); //turns the letter we send into something the arduino can read

    }
    else{
        qDebug() << " Couldn't write to serial.";
    }
}

void MainWindow::readSerial()
{

    QStringList buffer_split = serialBuffer.split(","); //delineates data by looking for a comma

    if(buffer_split.length() < 3){ //guarantees that the only value it reads is a value that's between two commas

   serialData = arduino->readAll(); //reads the raw data from the sensor
   serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
   serialData.clear();

    }else{
                serialBuffer = "";

                qDebug() << buffer_split << "\n" ; //creates a new line after each read value
                parsed_data = buffer_split[1]; //splits the raw data into readable values based on the commas
                force_value = (parsed_data.toDouble()); //changes our array into a double that the LCD can handle
                parsed_data = QString::number(force_value, 'g', 4); //limits the digits to 4
                MainWindow::updateForce(parsed_data); //sends our parsed data to the LCD to be displayed
                MainWindow::on_recordData_clicked(parsed_data); //saves data

    }


}


void MainWindow::updateForce(QString sensor_reading) //updating the LCD in the GUI
{
    ui->forceNumber->display(sensor_reading);
 }


void MainWindow::on_recordData_clicked(QString feltForce)//exporting data as a file
{
    if(ui->yesForce->isChecked())
    {
        QString filename="C:/Users/Jamie Kunnappally/Documents/Spring 2017 WOOHOO/Senior Design/ForceTest.txt";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << "Force Felt: " + ui->fingerSelect->currentText() + feltForce << endl;
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
