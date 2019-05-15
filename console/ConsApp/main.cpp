#include <QCoreApplication>
#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpClient obj;

    obj.Porta = 44900;
    obj.Servidor = "172.16.5.239";
    obj.Open();
    char s[10] = "ola";
    obj.Send(3,s);

    return a.exec();
}
