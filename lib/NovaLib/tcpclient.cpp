#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent,
                     quint16 porta,
                     QString servidor)
    : QObject(parent)
    , socket(new QTcpSocket(this))
{
    Porta = porta;
    Servidor = servidor;
    Finalizar = false;
}

TcpClient::~TcpClient()
{
    Finaliza();
}

void TcpClient::startconnection()
{
    qDebug("TcpClient " Q_FUNC_INFO );
    connect(socket, &QAbstractSocket::connected, this, &TcpClient::connected);
    connect(socket, &QAbstractSocket::disconnected, this, &TcpClient::disconnected);
    connect(socket, &QIODevice::readyRead, this, &TcpClient::readData);
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpClient::displayError);
    Open();

    // Ao Criar este thread, para enviar os dados via LoopPacote(),
    // a classe deixa de receber os dados do Server
    //QThread *thread = new QThread();
    //moveToThread(thread);

    //QObject::connect(thread, SIGNAL(started()), this, SLOT(LoopPacote()));
    //thread->start();
}

void TcpClient::LoopPacote()
{
    qDebug("TcpClient " Q_FUNC_INFO );
    for (;!Finalizar;)
    {
        LockSend.lock();
        if(!ListPacotes.isEmpty())
        {
            QByteArray data = ListPacotes.at(0);

            qDebug() << "send... " << data;

            Send(data);
            ListPacotes.removeAt(0);
        }
        LockSend.unlock();
        QThread::msleep(10);
    }
    emit finished();
}

bool TcpClient::Open()
{
    bool ret = false;
    try
    {
        socket->connectToHost(Servidor, Porta);
        if(!socket->waitForConnected(5000))
        {
            qDebug() << "socket Error: " << socket->errorString();
            Finaliza();
        }
        else {
            ret = true;
        }
        return ret;
    }
    catch (std::exception & e)
    {
        qDebug() << "connection Error: " << e.what();
        Finaliza();
        return ret;
    }
}

void TcpClient::ReadDataUser()
{
    QByteArray dados;
    while (socket->bytesAvailable())
    {
        dados.append(socket->readAll());
    }
    QString s_data = QString::fromStdString(dados.data());
    qDebug() << "ReadDataUser " << s_data;
}

/*void TcpClient::CloseSocket()
{
    qDebug() << "close socket" << socket;
    if(socket != nullptr)
    {
        socket->close();
        delete socket;
        socket = nullptr;
    }
}*/

void TcpClient::Finaliza()
{
    Finalizar = true;
}


void TcpClient::SetPacote(int len, char *p)
{
    QByteArray data(p, len);
    Send(len, p);
}

qint64 TcpClient::Send(QByteArray data)
{
    qint64 bytesSend = socket->write(data.data(), data.size());
    socket->flush();
    while(socket->bytesToWrite() > 0)
    {
        bytesSend += socket->write( data.data()+bytesSend, data.size()-bytesSend );
        socket->flush();
        socket->waitForBytesWritten();
    }
    return bytesSend;
}

qint64 TcpClient::Send(int len, char *p)
{
    try
    {
        QByteArray data(p, len);
        qint64 bytesSend = socket->write(data.data(), data.size());
        socket->flush();
        while(socket->bytesToWrite() > 0)
        {
            bytesSend += socket->write( data.data()+bytesSend, data.size()-bytesSend );
            socket->flush();
            socket->waitForBytesWritten();
        }
        return bytesSend;
    }
    catch (std::exception & e)
    {
        qDebug() << "write Error: " << e.what();
        Finaliza();
        //TODO: Verificar se é necessário chamar o signal disconnected()
        //disconnected();
        return -1;
    }
}

void TcpClient::connected()
{
    qDebug() << "Class TcpClient - Connected!";
}

void TcpClient::disconnected()
{
    qDebug() << "Class TcpClient - disconnected!";
}

void TcpClient::readData()
{
    ReadDataUser();
}

void TcpClient::bytesWritten(qint64 bytes)
{
    qDebug() << "bytesWritten " << bytes;
}


void TcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << "The host was not found. Please check the host name and port settings.";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << "The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.";
            break;
        default:
            qDebug() << "The following error occurred: " << socket->errorString();
           break;
    }
}
