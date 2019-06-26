#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "novalib_global.h"

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMutex>
#include <QThread>
#include <QMetaProperty>
#include <QTimer>

class NOVALIBSHARED_EXPORT TcpClient : public QObject
{
    Q_OBJECT

public:

    TcpClient(QObject *parent = nullptr,quint16 porta = 0,QString servidor = nullptr);
    ~TcpClient();

    quint16 Porta = 0;
    QString Servidor;

    bool Open();

    void ReadDataUser();
    void SetPacote(int len, char *p);

public slots:

    void startconnection();
    void connected();
    void disconnected();
    void readData();
    void bytesWritten(qint64 bytes);
    void displayError(QAbstractSocket::SocketError socketError);
    void finalizar();
    qint64 Send(int len, char *p);
    qint64 Send(QByteArray data);

signals:

    void encerra();
    void finished();
    qint64 enviaPacote(QByteArray data);

private:
    QMutex LockSend;

    QTcpSocket *socket = nullptr;
};

#endif // TCPCLIENT_H
