#ifndef WRAPPERTCPCLIENT_H
#define WRAPPERTCPCLIENT_H

#include "tcpclient.h"

#ifdef __cplusplus
extern "C" {
#endif

extern __declspec(dllexport) TcpClient* CreateTcpClient();

extern __declspec(dllexport) void Open(TcpClient* a_pObject,char *servidor, int porta);

extern __declspec(dllexport) void Send(TcpClient* a_pObject,int len, char *p);

extern __declspec(dllexport) void ReadData(TcpClient* a_pObject);

#ifdef __cplusplus
}
#endif

#endif // WRAPPERTCPCLIENT_H
