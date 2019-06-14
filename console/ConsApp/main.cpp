#include <QCoreApplication>
#include "tcpclient.h"
#include "mythread.h"

void TestApi()
{
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << " main thread " << QCoreApplication::instance()->thread();

    /*
    TcpClient obj;

    obj.Porta = 44900;
    obj.Servidor = "172.16.5.239";
    obj.Open();
    char s[10] = "ola";
    obj.Send(3,s);
    */

    MyThread thread1("A"), thread2("B"), thread3("C");

    thread1.start();
    thread2.start();
    thread3.start();

    return a.exec();
}
