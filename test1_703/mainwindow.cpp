 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QComboBox>
#include <QIODevice>//??
#include <qfile.h>
#include <QTextSTream>
#include <qtimer.h>
#include <qtextedit>
#include <qstring.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //[解決]TODO:查詢serialport資訊設定，並且顯示在combobox選單中~~~
    //目前困難:電腦無serialport必須從USB轉接serialport，並對接
    //才能將QT視窗與自己電腦中終端機通訊。
    //![0]測試中
    ui->tB_log->append(QString("檢測串口設備列表:"));
       foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {

           serial.setPort(info);
             if (serial.open(QIODevice::ReadWrite)){

            ui->comB1_port->addItem(info.portName());
            ui->comB1_port_2->addItem(info.portName());
            ui->tB_log->append(info.portName());
            ui->tB_log->append(info.description());
            ui->tB_log->append(info.manufacturer());

            serial.close();
        }

       //![0]

        //限制buffer輸入為數字且最高大小
      QValidator *validator=new QIntValidator(0,4001,this); // 0-100 only accept number
       ui->lineEdit->setValidator(validator);
       //限制傳輸訊息只能有英數及符號(不能有中文字母
      QRegExp regx("[a-zA-Z0-9+*-/.,\]+$");
      QValidator *validator2 = new QRegExpValidator(regx, this );
       ui->lineEdit_send->setValidator( validator2 );

       serialTimer = new QTimer(this);
       connect(serialTimer, SIGNAL(timeout()), this, SLOT(serialTimerUpdate()));

       //將serialport readread訊號 作為接收reci()函式的觸發
    connect(&serial,SIGNAL(readyRead()),this,SLOT(reci()));


/*
    //pBtn_send = new QPushButton(tr("send"),this);
    //上行會新增一"pushButton"元件'pBtn_send';與'ui->pBtn_send'不同
   // connect(ui->pBtn_send   , &QPushButton::clicked, this, &MainWindow::sendData);//可用但有重複觸發問題
   // connect(ui->btn_refresh, &QPushButton::clicked, this, &MainWindow::configChange);//可用但有重複觸發問題

    //connect(ui->btn,signal(pressed()),this,slot(on_btn_pressed()));
    //connect(ui->btn,signal(pressed()),this,slot(on_btn_pressed()),qt::uniqueconnection);
    //connect(ui->btn,signal(pressed()),this,slot(btn_pressed()));*/
}
}

MainWindow::~MainWindow()
{
    m_reader.close();
    serial.close();
    delete ui;
}



//! [1] 傳送資料_使用Serialport傳送資料，
//! 同時判斷是否可傳送(通道淨空、傳送暫存器清空)，
//! {待確定是否要再檢查一次設定(在傳送前)}

void MainWindow::sendData(int value){//暫時沒有用
ui->tB_log->append("選擇測試~~~");
ui->tB_log->append("serial  port : "+this->ui->comB1_port->currentText());
ui->tB_log->append("Baud Rate : "+this->ui->comB2_BR->currentText());
ui->tB_log->append("Paity Bit    : "+this->ui->comB3_PB->currentText());
ui->tB_log->append("Data  Bit    : "+this->ui->comB4_DB->currentText());
ui->tB_log->append("Stop  Bit    : "+this->ui->comB5_SB->currentText());

}
//![1]

//![2]
//! 當combobox改變選項時，呼叫此slot改變serialport
void MainWindow::configChange(int value){//暫時沒有用
temp+=1;
ui->tB_log->append(QString("test "));
}//! [2]


//! [3]
void MainWindow::reci(){
                    //單位msec
    serialTimer->start(1000);//收到數據時，開始計時!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!服務終止

         //while (!serial.atEnd()) {
            //Rx = serial.read(1000);
              Rx = serial.readAll();
         // }


    // Rx=serial.readAll();
     QString reciTEXT=Rx.toStdString().data();

     /*加上標頭判斷式(預想為使用uincode控制字元一碼作為傳送開頭依據(因為有時封包只有一個自元被傳送)
     ，或著使用buff將傳入字元收集(依據head & tail))  :新建暫存器
     QString str = "hello world";
     bool test = str.contains("world",Qt::CaseSensitive);   //test=true
     bool test1 = str.startsWith("hello",Qt::CaseSensitive); //test1=true
     bool test2 = str.endsWith("WORLD",Qt::CaseInsensitive); //test2=true
     */


     //------------------------------------------------------------------------//
     //下面是表示在UI上
     ui->tB_rx->append(reciTEXT.toStdString().data());
     ui->tE_rx->setText(ui->tE_rx->toPlainText()+Rx);//累加所有收到訊息
     //ui->tE_rx->setText(Rx);


    //trans();
    //temp暫時 下面是儲存log用 todo:誰送的 誰收的 標示出不同則指令

     QFile file("C:\\Users\\mc.wang\\Documents\\test_file\\test.txt");
     if(thisPort.toStdString().data()=="COM9"){
       file.close();
       QFile file("C:\\Users\\mc.wang\\Documents\\test_file\\qt_com9.txt");
   }else if(thisPort.toStdString().data()=="COM8"){
       file.close();
       QFile file("C:\\Users\\mc.wang\\Documents\\test_file\\qt_com8.txt");
   }



    if(file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)){
        QTextStream streamFileOut(&file);

        //streamFileOut<<QString("thisPort");//遇到問題:recive為多次、破碎的沒辦法判斷最前面為何。目前先加在發送訊息方

        //streamFileOut<<QString(reciTEXT.toStdString().data());
        //streamFileOut<<QString(reciTEXT);//->同tB_rx
        streamFileOut<<reciTEXT.toStdString().data();
        //streamFileOut<<reciTEXT;


        //streamFileOut<<QLatin1String(reciTEXT);
        //streamFileOut<<QStringLiteral(Rx);
        streamFileOut.flush();
    }
    file.close();


}//! [3]

//! [4]
void MainWindow::trans( ){//暫時沒用

    QFile file("C:\\Users\\mc.wang\\Documents\\test_file\\test.txt");

    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream streamFileOut(&file);
        /*streamFileOut.setCodec("UTF-8");
        streamFileOut<<QString("測試...").toUtf8();
        streamFileOut<<QString::fromUtf8("測試...");
        streamFileOut<<QLatin1String("測試..."); //亂碼æ¸¬è©¦...
        streamFileOut<<QStringLiteral("測試...");
        */

        streamFileOut<<QString("test1234...");
        streamFileOut<<QLatin1String("test1234...");
        streamFileOut<<QStringLiteral("test1234...");
        streamFileOut.flush();
    }
    file.close();


}//! [4]



//! [5]
void MainWindow::on_pBtn_send_clicked()
{
    serial.setPort(info);
    //! [5.1]
    //! 將資訊顯示在log上
    ui->tB_log->append("~~~選擇測試~~~");
    ui->tB_log->append("serial port  : "+this->ui->comB1_port->currentText());
    //ui->tB_log->append("serial port R: "+this->ui->comB1_port_2->currentText());
    ui->tB_log->append("Baud Rate : "+this->ui->comB2_BR->currentText());
    ui->tB_log->append("Paity Bit    : "+this->ui->comB3_PB->currentText());
    ui->tB_log->append("Data  Bit    : "+this->ui->comB4_DB->currentText());
    ui->tB_log->append("Stop  Bit    : "+this->ui->comB5_SB->currentText());
    //! [5.1]

    //! [5.2]
    //! 驗證是否是目標Port
    //!
    setthePort();//設定port子程式

/*
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
    if(info.portName()==this->ui->comB1_port->currentText()){
    //! 建立傳送通道
    ui->tB_log->append("找到 XXXX");
    com_info=info;
    //! [5.2]
    }


    m_reader.setPort(com_info);
    if(m_reader.open(QIODevice::ReadWrite))
        {
             //qDebug() << "m_reader.open(QIODevice::ReadWrite)";
            m_reader.setBaudRate(QSerialPort::Baud9600);
            m_reader.setParity(QSerialPort::NoParity);
            m_reader.setDataBits(QSerialPort::Data8);
            m_reader.setStopBits(QSerialPort::OneStop);
            m_reader.setFlowControl(QSerialPort::NoFlowControl);

            m_reader.clearError();
            m_reader.clear();
            //connect(&m_reader, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

            //bool test=m
        }


    
    }
*/

}//! [5]


void MainWindow::on_btn_refresh_2_clicked()//DATA Send~~//TODO:可能還要再加寫檔案的，去掉送出的部分
{   //serial.write(thisPort.toStdString().data());
    // serial.write(":");//head
    //serial.write(ui->lineEdit_send->text().toStdString().data());
    //serial.write(thisPort.toLatin1());

    serial.write(ui->textEdit->toPlainText().toLatin1());//????????????????????????????????

    //ui->tB_tx->append(ui->lineEdit_send->text().toStdString().data()+'n');
    // serial.write("_respond_");//tail
    // serial.write("\n");

    QString sendTEXT=ui->lineEdit_send->text();
    ui->tB_tx->append(sendTEXT.toStdString().data());

    Tx=ui->textEdit->toPlainText().toLatin1();//?????????
    ui->tB_tx->append(Tx);
}

void MainWindow::on_btn_refresh_3_clicked()//close PORT & clean text
{
    //清除
    serial.close();
    //serialRx.close();
    ui->tB_rx->clear();
    ui->tB_tx->clear();
    ui->tE_rx->clear();

}


void MainWindow::setthePort(){

    serial.clear();
    //serialRx.clear();

    serial.close();
    //serialRx.close();

    thisPort=this->ui->comB1_port->currentText();
    serial.setPortName(thisPort);
    //serialRx.setPortName(this->ui->comB1_port_2->currentText());
    serial.open(QIODevice::ReadWrite);
    //serialRx.open(QIODevice::ReadOnly);

    /************************************************************/
    switch (ui->comB2_BR->currentIndex()){//沒有比115200更高的了
        case 0: //115200
            serial.setBaudRate(QSerialPort::Baud115200);
            break;
        case 1://19200
            serial.setBaudRate(QSerialPort::Baud19200);
            //serialRx.setBaudRate(QSerialPort::Baud19200);
            break;
        case 2://9600
            serial.setBaudRate(QSerialPort::Baud9600);
            //serialRx.setBaudRate(QSerialPort::Baud9600);
            break;
        case 3://4800
            serial.setBaudRate(QSerialPort::Baud4800);
            //serialRx.setBaudRate(QSerialPort::Baud4800);
           break;
        case 4://2400
            serial.setBaudRate(QSerialPort::Baud2400);
            //serialRx.setBaudRate(QSerialPort::Baud2400);
            break;

    }
    /*************************************************/
    switch (ui->comB3_PB->currentIndex()){
        case 0:
           serial.setParity(QSerialPort::NoParity);
           //serialRx.setParity(QSerialPort::NoParity);
            break;
        case 1:
            serial.setParity(QSerialPort::OddParity);
            //serialRx.setParity(QSerialPort::OddParity);
            break;
        case 2:
            serial.setParity(QSerialPort::EvenParity);
            //serialRx.setParity(QSerialPort::EvenParity);
            break;
    }
    /**************************************************/
    switch (ui->comB4_DB->currentIndex()) {
        case 0:
            serial.setDataBits(QSerialPort::Data7);
            //serialRx.setDataBits(QSerialPort::Data7);
            break;
        case 1:
            serial.setDataBits(QSerialPort::Data8);
            //serialRx.setDataBits(QSerialPort::Data8);
            break;
    }
    /**************************************************/
    switch (ui->comB5_SB->currentIndex()) {
        case 0:
            serial.setStopBits(QSerialPort::OneStop);
            //serialRx.setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            serial.setStopBits(QSerialPort::OneAndHalfStop);
            //serialRx.setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            serial.setStopBits(QSerialPort::TwoStop);
            //serialRx.setStopBits(QSerialPort::TwoStop);
            break;

    }
    /***************************************************/

    /*
    int BaudRate;//B鮑率={28800,19200,9600,4800,2400,1200,600,300,110}
    int paityBit;//奇偶校正位={1,0}
    int dataBit;//資料位元數={7,8}
    int stopBit;//停止位元={1,1.5,2} ?int?
    int bufferSize;//暫存器大小={???}
    */
}


void MainWindow::on_btn_refresh_clicked()
{


    ui->comB1_port->clear();
    ui->comB1_port->addItem(thisPort);


    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
     {

        m_reader.setPort(info);
          if (m_reader.open(QIODevice::ReadWrite)){

         ui->comB1_port->addItem(info.portName());
         //ui->comB1_port_2->addItem(info.portName());
         ui->tB_log->append(info.portName());
         ui->tB_log->append(info.description());
         ui->tB_log->append(info.manufacturer());

         m_reader.close();
     }

}
}


void MainWindow::serialTimerUpdate(){

     serialTimer->stop();

}
/*


1
12
123
1234
12345
123456
1234567
12345678
123456789
1234567890
123456789
12345678
1234567
123456
12345
1234
123
12
1
12
123
1234
12345
123456
1234567
12345678
123456789
1234567890
  234567890
    34567890
      4567890
        567890
          67890
            7890
              890
                90
                  0
                90
              890
            7890
          67890
        567890
      4567890
    34567890
  234567890
1234567890
123456789
12345678
1234567
123456
12345
1234
123
12
1


*/

void MainWindow::on_pBtn_send_2_clicked()
{
    QString comtemp;
    comtemp="AT+UWAPCA=0,4\r\n"
            "   ";
     serial.write(comtemp.toLatin1());

     comtemp="AT+UWAPC=0,0,0\r\n";
      serial.write(comtemp.toLatin1());

      comtemp="AT+UWAPC=0,2,\"UBXWifi\"\r\n";
       serial.write(comtemp.toLatin1());

       comtemp="AT+UWAPC=0,4,1\r\n";
        serial.write(comtemp.toLatin1());

        comtemp="AT+UWAPC=0,5,2,2\r\n";
         serial.write(comtemp.toLatin1());

         comtemp="AT+UWAPC=0,8,\"mypassword\"\r\n";
          serial.write(comtemp.toLatin1());

          comtemp="AT+UWAPC=0,100,1\r\n";
           serial.write(comtemp.toLatin1());

           comtemp="AT+UWAPC=0,101,192.168.2.1\r\n";
            serial.write(comtemp.toLatin1());

            comtemp="AT+UWAPC=0,102,255.255.255.0\r\n";
             serial.write(comtemp.toLatin1());

             comtemp="AT+UWAPC=0,103,192.168.2.1\r\n";
              serial.write(comtemp.toLatin1());

              comtemp="AT+UWAPC=0,106,1\r\n";
               serial.write(comtemp.toLatin1());

               comtemp="AT+UWAPCA=0,3\r\n";
                serial.write(comtemp.toLatin1());

                comtemp="AT+UDSC=1,1,8080\r\n";
                 serial.write(comtemp.toLatin1());

                 comtemp="ATO1\r\n";
                  serial.write(comtemp.toLatin1());


    /*
    //On device #1 (Access Point):
    //Network part using Static for AP and DHCP for clients

    //Deactivate network id 0.


    //Not active on startup.


    //Set SSID for the Network.


    //Set Channel 1 for the Network.


    //Set WPA2 Security for the Network.


    //Use Password “my passwor


    //Static IP address for Access Point.


    //Network IP address.


    //Network Subnet mask.


    //Network Gateway address.


    //Enable DHCP for connected Clients. DHCP Server will provide addresses according to the
    //following formula: (Static address & subnet mask) + 100. The first client will get the IP address
    //192.168.2.100.


    //Activate Wi-Fi Access Point configuration.


    //TCP and data part
    //Set server configuration id 1, using TCP and port 8080.


    //Enter Data Mode
    //Enter Data Mode to send data.





            AT+UWAPCA=0,4

            AT+UWAPC=0,0,0

            AT+UWAPC=0,2,”UBXWifi”

            AT+UWAPC=0,4,1

            AT+UWAPC=0,5,2,2

            AT+UWAPC=0,8,”my password”

            AT+UWAPC=0,100,1

            AT+UWAPC=0,101,192.168.123.100

            AT+UWAPC=0,102,255.255.255.0

            AT+UWAPC=0,103,192.168.123.100

            AT+UWAPC=0,106,1

            AT+UWAPCA=0,3

            AT+UDSC=1,1,8080

            ATO1
      */

}

void MainWindow::on_pBtn_send_3_clicked()
{   QString comtemp;
    comtemp="+++\r\n";
     serial.write(comtemp.toLatin1());
}
