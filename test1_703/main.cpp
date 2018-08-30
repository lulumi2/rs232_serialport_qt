#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <QSerialPort>      //串口控制
#include <QSerialPortInfo>  //串口資訊

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    //![0]在Debug看Port資訊
    //mainwindow->tB_log->append(QString("檢測到端口列表:"));
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
           QSerialPort serial;
           serial.setPort(info);
             if (serial.open(QIODevice::ReadWrite)){
            qDebug() << "Name : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();
            qDebug() << "Serial Number: " << info.serialNumber();
            qDebug() << "System Location: " << info.systemLocation();

           //main window->comboBox_serialport->addItem(info.portName());
           // ui->textEdit_recvSerial->append(info.portName());
           // ui->textEdit_recvSerial->append(info.description());
           // ui->textEdit_recvSerial->append(info.manufacturer());
            serial.close();
            }
        }
    //![0]

    w.show();
    return a.exec();
}
