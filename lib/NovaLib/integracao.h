#ifndef INTEGRACAO_H
#define INTEGRACAO_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QCoreApplication>

#include "tcpclient.h"

class Integracao : public QObject
{
    Q_OBJECT

public:
    Integracao(QObject *parent = nullptr);
    ~Integracao();

    int InitClient(int porta, char *servidor);
    void FinalizaClient();
    void EnviaPacote(int len, char *p);

    void (*OnConectado)() = nullptr;

public slots:

    void conectado();

    void destroyed();
    void errorString(QString s);

private:

    QThread *thread = nullptr;
    TcpClient *client = nullptr;

    QCoreApplication *appMain = nullptr;

    char **SetArgv(int qtos, ...);
};

#endif // INTEGRACAO_H
