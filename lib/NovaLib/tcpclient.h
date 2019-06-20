#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "novalib_global.h"

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QString>
#include <QEventLoop>
#include <QMutex>
#include <QThread>

//class QTcpSocket;
//class QNetworkSession;

class NOVALIBSHARED_EXPORT TcpClient : public QObject
{
    Q_OBJECT

public:

    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    QList<QByteArray> ListPacotes;

    quint16 Porta = 0;
    QString Servidor;

    QTcpSocket *socket = nullptr;

    bool Open();
    bool Open(QString servidor, quint16 porta);
    void Init(QString servidor, quint16 porta);
    void CloseSocket();
    qint64 Send(int len, char *p);
    void Send(QByteArray data);
    void ReadDataUser();
    void SetPacote(int len, char *p);

public slots:

    void startconnection();
    void connected();
    void disconnected();
    void readData();
    void bytesWritten(qint64 bytes);
    void displayError(QAbstractSocket::SocketError socketError);
    void LoopPacote();

private:
    QMutex LockSend;
    //QTcpSocket *socket = nullptr;
};

#endif // TCPCLIENT_H
