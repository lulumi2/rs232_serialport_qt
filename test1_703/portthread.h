#ifndef PORTTHREAD_H
#define PORTTHREAD_H

#include <QThread>


class portThread : public QThread
{
    Q_OBJECT
public:
    portThread(QObject *parent = 0) : QThread(parent){ }
protected:
    void run(){
        for(int i = 0; i < 3000000000; i++){
            int a = 0;
        }
        emit done();
    }
signals:
    void done();
};

#endif // PORTTHREAD_H
