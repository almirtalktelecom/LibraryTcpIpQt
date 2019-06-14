#include "mythread.h"

#include <QFile>
#include <QTextStream>

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    restart = false;
    abort = false;
}

/*void MyThread::run()
{
    qDebug() << "Thread started " << this->thread();
    tcpClient = new TcpClient;

    tcpClient->Porta = Porta;
    tcpClient->Servidor = Servidor;

    this->moveToThread(tcpClient->socket->thread());

    connect(tcpClient->socket, &QAbstractSocket::connected, this, &MyThread::connected);
    connect(tcpClient->socket, &QAbstractSocket::disconnected, this, &MyThread::disconnected);
    connect(tcpClient->socket, &QIODevice::readyRead, this, &MyThread::readData, Qt::DirectConnection);
    connect(tcpClient->socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    //connect(tcpClient->socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &MyThread::displayError);
    tcpClient->Open();
}*/

void MyThread::run()
{
    // thread starts here
    socket = new QTcpSocket();
    qDebug() << "Thread started - " << this->thread();
    qDebug() << "socket thread  - " << socket->thread();

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.
    connect(socket, &QAbstractSocket::connected, this, &MyThread::connected);
    connect(socket, &QAbstractSocket::disconnected, this, &MyThread::disconnected);
    connect(socket, &QIODevice::readyRead, this, &MyThread::readData, Qt::DirectConnection);
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &MyThread::displayError);

    try
    {
        socket->connectToHost(Servidor, Porta);
        if(!socket->waitForConnected(5000))
        {
            qDebug() << "socket Error: " << socket->errorString();
            CloseSocket();
            return;
        }
    }
    catch (std::exception & e)
    {
        qDebug() << "connection Error: " << e.what();
        CloseSocket();
        return;
    }

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void MyThread::connected()
{
    qDebug() << "Class MyThread - Connected!";
}

void MyThread::disconnected()
{
    qDebug() << "Class MyThread - disconnected!";
}

void MyThread::readData()
{
    ReadDataUser();
}

void MyThread::bytesWritten(qint64 bytes)
{
    qDebug() << "bytesWritten " << bytes;
}

void MyThread::displayError(QAbstractSocket::SocketError socketError)
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

qint64 MyThread::Send(int len, char *p)
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

void MyThread::ReadDataUser()
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

void MyThread::CloseSocket()
{
    qDebug() << "close socket" << socket;
    if(socket != nullptr)
    {
        socket->close();
        socket->deleteLater();
        socket = nullptr;
    }
}
