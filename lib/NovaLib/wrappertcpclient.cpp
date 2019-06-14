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
/// \brief InitClient
///
void InitClient(MyThread* thread)
{
    int len = 1;
    char **argv = SetArgv(len, "LibraryQTCti");

    QCoreApplication app(len, argv);

    thread->Porta = 44900;
    thread->Servidor = "172.16.5.239";

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
