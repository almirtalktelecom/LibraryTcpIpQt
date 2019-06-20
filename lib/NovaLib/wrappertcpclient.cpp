#include "wrappertcpclient.h"

char **SetArgv(int qtos, ...);

///
/// \brief CreateTcpClient
/// \return
///
TcpClient* CreateTcpClient()
{
    return new TcpClient();
}

///
/// \brief InitClientExt
/// \param client
/// \param porta
/// \param servidor
/// \return
///
int InitClientExt(TcpClient* client,int porta, char *servidor)
{
    if(client != nullptr)
    {
        int len = 1;
        char **argv = SetArgv(len, "LibraryQTCti");
        QCoreApplication app(len, argv);
        QThread *thread = new QThread ();

        client->Porta = quint16(porta);
        client->Servidor = servidor;

        client->moveToThread(thread);
        QObject::connect(thread, &QThread::started, client, &TcpClient::startconnection);
        //QObject::connect(thread, &QThread::started, client, &TcpClient::LoopPacote);

        QObject::connect(thread, &QThread::finished, client, &TcpClient::deleteLater);
        QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
        return app.exec();
    }
    return -1;
}

void SetPacote(TcpClient* client,int len, char *p)
{
    if(client!= nullptr)
    {
        //client->SetPacote(len, p);
        client->Send(len, p);
    }
}

///
/// \brief Send
/// \param client
/// \param len
/// \param p
///
void Send(TcpClient* client,int len, char *p)
{
    if(client!= nullptr)
    {
        client->Send(len, p);
    }
}

///
/// \brief InitClient
///
void InitClient(MyThread* thread,int porta, char *servidor)
{
    int len = 1;
    char **argv = SetArgv(len, "LibraryQTCti");

    QCoreApplication app(len, argv);

    thread->Porta = quint16(porta);
    thread->Servidor = servidor;

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

    app.exec();
}

///
/// \brief CreateMyThread
/// \return
///
MyThread* CreateMyThread()
{
    return new MyThread();
}

///
/// \brief SendExt
/// \param thread
/// \param len
/// \param p
///
void SendExt(MyThread* thread,int len, char *p)
{
    if(thread!= nullptr)
    {
        thread->Send(len, p);
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
