#include "tcpclient.h"

///
/// \brief TcpClient::TcpClient
/// \param parent
/// \param porta
/// \param servidor
///
TcpClient::TcpClient(QObject *parent,
                     quint16 porta,
                     QString servidor)
    : QObject(parent)
    , socket(new QTcpSocket(this))
{
    Porta = porta;
    Servidor = servidor;
}

///
/// \brief TcpClient::~TcpClient
///
TcpClient::~TcpClient()
{
    finalizar();
}

///
/// \brief TcpClient::startconnection
///
void TcpClient::startconnection()
{
    qDebug("TcpClient startconnection");
    connect(socket, &QAbstractSocket::connected, this, &TcpClient::connected);
    connect(socket, &QAbstractSocket::disconnected, this, &TcpClient::disconnected);
    connect(socket, &QIODevice::readyRead, this, &TcpClient::readData);
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpClient::displayError);
    Open();
}

///
/// \brief TcpClient::Open
/// \return
///
bool TcpClient::Open()
{
    bool ret = false;
    try
    {
        socket->connectToHost(Servidor, Porta);
        if(!socket->waitForConnected(5000))
        {
            qDebug() << "socket Error: " << socket->errorString();
            finalizar();
        }
        else {
            ret = true;
        }
        return ret;
    }
    catch (std::exception & e)
    {
        qDebug() << "connection Error: " << e.what();
        finalizar();
        return ret;
    }
}

///
/// \brief TcpClient::ReadDataUser
///
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

///
/// \brief TcpClient::finalizar
///
void TcpClient::finalizar()
{
    qDebug() << "TcpClient::finalizar()";
    socket->close();
}

///
/// \brief TcpClient::Send
/// \param len
/// \param p
/// \return
///
qint64 TcpClient::Send(int len, char *p)
{
    try
    {
        QByteArray data(p, len);
        return Send(data);
    }
    catch (std::exception & e)
    {
        qDebug() << "write Error: " << e.what();
        finalizar();
        //TODO: Verificar se é necessário chamar o signal disconnected()
        //disconnected();
        return -1;
    }
}

///
/// \brief TcpClient::Send
/// \param data
/// \return
///
qint64 TcpClient::Send(QByteArray data)
{
    LockSend.lock();
    qint64 bytesSend = socket->write(data.data(), data.size());
    socket->flush();
    while(socket->bytesToWrite() > 0)
    {
        bytesSend += socket->write( data.data()+bytesSend, data.size()-bytesSend );
        socket->flush();
        socket->waitForBytesWritten();
    }
    LockSend.unlock();
    return bytesSend;
}

///
/// \brief TcpClient::connected
///
void TcpClient::connected()
{
    qDebug() << "Class TcpClient - Connected!";
}

///
/// \brief TcpClient::disconnected
///
void TcpClient::disconnected()
{
    qDebug() << "Class TcpClient - disconnected!";
}

///
/// \brief TcpClient::readData
///
void TcpClient::readData()
{
    ReadDataUser();
}

///
/// \brief TcpClient::bytesWritten
/// \param bytes
///
void TcpClient::bytesWritten(qint64 bytes)
{
    qDebug() << "bytesWritten " << bytes;
}

///
/// \brief TcpClient::displayError
/// \param socketError
///
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
