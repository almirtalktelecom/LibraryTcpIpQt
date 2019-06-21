#include "wrappertcpclient.h"

#define VERSAO 36

QThread *thread = nullptr;
TcpClient *client = nullptr;

char **SetArgv(int qtos, ...);

///
/// \brief CreateTcpClient
/// \return
///
TcpClient* CreateTcpClient(int porta, char *servidor)
{
    qDebug("Versão %d", VERSAO);
    return new TcpClient(nullptr, quint16(porta), servidor);
}

///
/// \brief InitClientExt
/// \param client
/// \param porta
/// \param servidor
/// \return
///
int InitClientExt(int porta, char *servidor)
{
    qDebug("Versão %d", VERSAO);

    int len = 1;
    char **argv = SetArgv(len, "LibraryQTCti");
    QCoreApplication app(len, argv);

    thread = new QThread();
    client = new TcpClient(nullptr, quint16(porta), servidor);

    //client->moveToThread(thread);
    //connect(client, SIGNAL(error(QString)), this, SLOT(errorString(QString)));

    QObject::connect(thread, SIGNAL(started()), client, SLOT(startconnection()));
    QObject::connect(client, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
    return app.exec();
}

///
/// \brief SetPacote
/// \param client
/// \param len
/// \param p
///
void SetPacote(int len, char *p)
{
    if(client != nullptr)
    {
        // Envia os dados via LoopPacote()
        //client->SetPacote(len, p);

        // Envia os dados diretamente pelo socket
        client->Send(len, p);
    }
}

///
/// \brief SetArgv
/// \param qtos
/// \return
///
char **SetArgv(int qtos, ...)
{
    va_list args;
    int i;
    char **argv = (char **) malloc(size_t(qtos+1) * sizeof(char*));
    char *temp;
    va_start(args, qtos);
    for (i = 0; i < qtos; i++) {
        temp = va_arg(args, char*);
        argv[i] = (char *) malloc(sizeof(temp));
        argv[i] = temp;
    }
    argv[i] = nullptr;
    va_end(args);
    return argv;
}
