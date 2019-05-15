#include "wrappertcpclient.h"

TcpClient* CreateTcpClient()
{
    return new TcpClient();
}

void Open(TcpClient* a_pObject,char *servidor, int porta)
{
    if(a_pObject!= nullptr)
    {
        a_pObject->Open(servidor, quint16(porta));
    }
}

void Send(TcpClient* a_pObject,int len, char *p)
{
    if(a_pObject!= nullptr)
    {
        a_pObject->Send(len, p);
    }
}

void ReadData(TcpClient* a_pObject)
{
    if(a_pObject!= nullptr)
    {
        a_pObject->ReadDataUser();
    }
}


