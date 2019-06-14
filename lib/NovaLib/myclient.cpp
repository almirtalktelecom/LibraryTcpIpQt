#include "myclient.h"
#include "mythread.h"

MyClient::MyClient(QObject *parent) :
    QTcpSocket(parent)
{
}

void MyClient::StartClient(quint16 porta, QString servidor)
{
    MyThread *thread = new MyThread(this);

    thread->Porta = porta;
    thread->Servidor = servidor;

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

