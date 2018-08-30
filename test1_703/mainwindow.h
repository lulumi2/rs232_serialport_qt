#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>      //串口控制
#include <QSerialPortInfo>  //串口資訊
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

struct portInfo//串口資料格式(自定義)
{
    portInfo(){}
    portInfo(QString s,int p, int i,int j,int k, int l)
    {  portNum=s;BaudRate=p;paityBit=i; dataBit=j;stopBit=k;bufferSize=l; }

   // QString GameIp;//遊戲主機IP
   // quint16 GamePort;//遊戲UDP綁定的端口

    QString portNum;//com1,com2,...
    int BaudRate;//B鮑率={28800,19200,9600,4800,2400,1200,600,300,110}
    int paityBit;//奇偶校正位={1,0}
    int dataBit;//資料位元數={7,8}
    int stopBit;//停止位元={1,1.5,2} ?int?
    int bufferSize;//暫存器大小={???}
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int temp;//temp//////


private:
    Ui::MainWindow *ui;
    portInfo portinfo1;
    QPushButton *pBtn_send;
    QTextBrowser *tB_log;
    QByteArray Tx,Rx;

 public:
    const QSerialPortInfo info;
    QSerialPortInfo com_info;//
    QSerialPort serial,serialRx;
    QSerialPort m_reader;// COM串口_讀資料 ????不確定是否有用????
    QString thisPort;
    QTimer *serialTimer;

public slots:

private slots:
      void sendData(int value);//傳送按鍵_須判斷是否可傳再將資料送出
      void configChange(int value);//修改參數_所有參數更新一次

      void setthePort();
      void trans();//介面上送出啟動signal，傳送時呼叫此slot
      void reci();//硬體接收到signal後，執行接收動作

      void on_pBtn_send_clicked();
      void on_btn_refresh_2_clicked();
      void on_btn_refresh_3_clicked();

      void on_btn_refresh_clicked();

      void serialTimerUpdate();//計時器到時呼叫，整合蒐集到片段
      void on_pBtn_send_2_clicked();
      void on_pBtn_send_3_clicked();
};

#endif // MAINWINDOW_H
