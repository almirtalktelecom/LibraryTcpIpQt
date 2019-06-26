#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "novalib_global.h"

#include <QTcpSocket>
#include <QMutex>
#include <QThread>

class NOVALIBSHARED_EXPORT TcpClient : public QObject
{
    Q_OBJECT

public:

    quint16 Porta = 0;
    QString Servidor;

    TcpClient(QObject *parent = nullptr,quint16 porta = 0,QString servidor = nullptr);
    ~TcpClient();

    bool Open();

public slots:

    void startconnection();
    void finaliza();

    void connected();
    void disconnected();
    void readyRead();
    void bytesWritten(qint64 bytes);

    void displayError(QAbstractSocket::SocketError socketError);

    qint64 Send(int len, char *p);
    qint64 Send(QByteArray data);

    void destroyed();

signals:

    void conectado();

    void setFinaliza();
    void finalizado();
    qint64 enviaPacote(QByteArray data);

    void socketError(QString s);

private:
    QMutex LockSocket;
    QTcpSocket *socket = nullptr;
};

#endif // TCPCLIENT_H
