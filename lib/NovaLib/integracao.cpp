#include "integracao.h"

///
/// \brief Integracao::Integracao
/// \param parent
///
Integracao::Integracao(QObject *parent) : QObject(parent)
{

}

///
/// \brief Integracao::~Integracao
///
Integracao::~Integracao()
{

}

///
/// \brief Integracao::InitClient
/// \param porta
/// \param servidor
/// \return
///
int Integracao::InitClient(int porta, char *servidor)
{
    int len = 1;
    char **argv = SetArgv(len, "LibraryQTCti");
    QCoreApplication app(len, argv);
    appMain = &app;

    thread = new QThread();
    client = new TcpClient(nullptr, quint16(porta), servidor);

    connect(client, SIGNAL(socketError(QString)), this, SLOT(errorString(QString)));
    connect(client, SIGNAL(conectado()), this, SLOT(conectado()));

    QObject::connect(thread, SIGNAL(started()), client, SLOT(startconnection()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(client, SIGNAL(setFinaliza()), client, SLOT(finaliza()));
    QObject::connect(client, SIGNAL(enviaPacote(QByteArray)), client, SLOT(Send(QByteArray)));

    ///
    /// \brief QObject::connect
    /// Aguarda o objeto encerrar
    ///
    QObject::connect(client, SIGNAL(finalizado()), this , SLOT(destroyed()));

    thread->start();
    return app.exec();
}

///
/// \brief Integracao::FinalizaClient
///
void Integracao::FinalizaClient()
{
    if(appMain != nullptr)
    {
        client->setFinaliza();

        /*
        // Espera o slot , bolar outra forma de wait
        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        appMain->quit();
        */
    }
}

void Integracao::conectado()
{
    if(OnConectado != nullptr)
    {
        OnConectado();
    }
}

///
/// \brief Integracao::EnviaPacote
/// \param len
/// \param p
///
void Integracao::EnviaPacote(int len, char *p)
{
    if(client != nullptr)
    {
        // Set os dados
        //client->SetPacote(len, p);
        // e chama o slot para enviar
        QByteArray data(p, len);
        emit client->enviaPacote(data);
    }
}

///
/// \brief Integracao::errorString
/// \param s
///
void Integracao::errorString(QString s)
{
    qDebug() << "errorString " << s;
}

///
/// \brief Integracao::destroyed
///
void Integracao::destroyed()
{
    qDebug() << "Integracao destroyed()";
    appMain->quit();
}

///
/// \brief SetArgv
/// \param qtos
/// \return
///
char **Integracao::SetArgv(int qtos, ...)
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
