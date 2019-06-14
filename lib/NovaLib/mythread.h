#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "tcpclient.h"

#include <QCoreApplication>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject *parent = nullptr);

    quint16 Porta = 0;
    QString Servidor;

    qint64 Send(int len, char *p);

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void connected();
    void disconnected();
    void readData();
    void bytesWritten(qint64 bytes);
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
    bool restart;
    bool abort;

    void ReadDataUser();
    void CloseSocket();

protected:
    void run() override;
};

#endif // MYTHREAD_H
