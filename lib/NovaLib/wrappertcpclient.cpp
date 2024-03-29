#include "wrappertcpclient.h"

#define VERSAO 80

Integracao *integracao = new Integracao();
QThread *thread = nullptr;
TcpClient *client = nullptr;

QCoreApplication *appMain = nullptr;

char **SetArgv(int qtos, ...);
void destroyed();

///
/// \brief InitClient
/// \param client
/// \param porta
/// \param servidor
/// \return
///
int InitClient(int porta, char *servidor)
{
    qDebug("Versão %d", VERSAO);

#ifdef INTEGRACAO

    //integracao = new Integracao();
    integracao->InitClient(porta, servidor);
    integracao->deleteLater();
    return 0;

#else

    int len = 1;
    char **argv = SetArgv(len, "LibraryQTCti");
    QCoreApplication app(len, argv);
    appMain = &app;

    thread = new QThread();
    client = new TcpClient(nullptr, quint16(porta), servidor);

    QObject::connect(thread, SIGNAL(started()), client, SLOT(startconnection()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(client, SIGNAL(setFinaliza()), client, SLOT(finaliza()));
    QObject::connect(client, SIGNAL(enviaPacote(QByteArray)), client, SLOT(Send(QByteArray)));

    thread->start();
    return app.exec();

#endif
}

///
/// \brief FinalizaClient
///
void FinalizaClient()
{
#ifdef INTEGRACAO

    if(integracao != nullptr)
    {
        integracao->FinalizaClient();
    }

#else

    if(appMain != nullptr)
    {
        emit client->setFinaliza();

        // Espera o slot , bolar outra forma de wait
        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        appMain->quit();
    }

#endif
}

///
/// \brief SetOnConectado
/// \param onConectado
///
void SetOnConectado(void onConectado())
{
    if(integracao != nullptr)
    {
        integracao->OnConectado = onConectado;
    }
}

///
/// \brief SetPacote
/// \param client
/// \param len
/// \param p
///
void EnviaPacote(int len, char *p)
{
#ifdef INTEGRACAO

    if(integracao != nullptr)
    {
        integracao->EnviaPacote(len, p);
    }

#else
    if(client != nullptr)
    {
        // Set os dados
        //client->SetPacote(len, p);
        // e chama o slot para enviar
        QByteArray data(p, len);
        emit client->enviaPacote(data);
    }
#endif
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
