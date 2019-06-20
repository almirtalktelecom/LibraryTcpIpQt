#include "tcpclient.h"

#include <QFile>
#include <QTextStream>
#include <QMetaProperty>

#define VERSAO 29

TcpClient::TcpClient(QObject *parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
{
    qDebug("Versão %d", VERSAO);
    qDebug() << "thread TcpClient " << thread();
}

TcpClient::~TcpClient()
{
    CloseSocket();
}

void TcpClient::startconnection()
{
    qDebug("TcpClient " Q_FUNC_INFO );
    connect(socket, &QAbstractSocket::connected, this, &TcpClient::connected);
    connect(socket, &QAbstractSocket::disconnected, this, &TcpClient::disconnected);
    connect(socket, &QIODevice::readyRead, this, &TcpClient::readData);
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpClient::displayError);
    Open(Servidor, Porta);


    QByteArray data("Almir dois", 10);
    socket->write(data.data(), data.size());
    socket->flush();
}

void TcpClient::LoopPacote()
{
    qDebug("TcpClient " Q_FUNC_INFO );
    for (;;)
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
}

void TcpClient::Init(QString servidor, quint16 porta)
{
    //QEventLoop loop;

    //qDebug("init loop");

    connect(socket, &QAbstractSocket::connected, this, &TcpClient::connected);
    connect(socket, &QAbstractSocket::disconnected, this, &TcpClient::disconnected);
    connect(socket, &QIODevice::readyRead, this, &TcpClient::readData);
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpClient::displayError);

    //qDebug("after connect");

    Open(servidor, porta);

    //qDebug("after open");

    //loop.exec();

    //qDebug("finish loop");
}

bool TcpClient::Open(QString servidor, quint16 porta)
{
    Porta = porta;
    Servidor = servidor;
    return Open();
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
            CloseSocket();
        }
        else {
            ret = true;
        }

        //QByteArray data("Almir", 5);
        //socket->write(data.data(), data.size());
        //socket->flush();

        return ret;
    }
    catch (std::exception & e)
    {
        qDebug() << "connection Error: " << e.what();
        CloseSocket();
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
    QString filename="D:\\Data.txt";

    QFile file( filename );
    if ( file.open(QIODevice::Append) )
    {
        QTextStream stream( &file );
        stream << s_data << endl;
        file.close();
    }
    qDebug() << s_data;
}

void TcpClient::CloseSocket()
{
    qDebug() << "close socket" << socket;
    if(socket != nullptr)
    {
        socket->close();
        delete socket;
        socket = nullptr;
    }
}

void TcpClient::SetPacote(int len, char *p)
{
    QByteArray data(p, len);

    qDebug() << "SetPacote... " << data;

    Send(len, p);

    //LockSend.lock();
    //ListPacotes.append(data);
    //LockSend.unlock();
}

void TcpClient::Send(QByteArray data)
{
    qint64 bytesSend = socket->write(data.data(), data.size());
    socket->flush();
    while(socket->bytesToWrite() > 0)
    {
        bytesSend += socket->write( data.data()+bytesSend, data.size()-bytesSend );
        socket->flush();
        socket->waitForBytesWritten();
    }
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
        CloseSocket();
        //TODO: Verificar se é necessário chamar o signal disconnected()
        //disconnected();
        return -1;
    }
}

void TcpClient::connected()
{
    qDebug() << "Class TcpClient - Connected!";

    /*QThread *thread = new QThread ();
    //moveToThread(thread);
    connect(thread, &QThread::started, this, &TcpClient::LoopPacote);
    thread->start();

    qDebug() << "Class TcpClient - liberou!";*/

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
