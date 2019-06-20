#ifndef WRAPPERTCPCLIENT_H
#define WRAPPERTCPCLIENT_H

#include <QCoreApplication>
#include <QObject>

#include "tcpclient.h"
#include "mythread.h"

#ifdef __cplusplus
extern "C" {
#endif

extern __declspec(dllexport) TcpClient* CreateTcpClient();

extern __declspec(dllexport) MyThread* CreateMyThread();

extern __declspec(dllexport) int InitClientExt(TcpClient* client,int porta, char *servidor);

extern __declspec(dllexport) void Send(TcpClient* client,int len, char *p);

extern __declspec(dllexport) void SetPacote(TcpClient* client,int len, char *p);

extern __declspec(dllexport) void InitClient(MyThread* thread,int porta, char *servidor);

extern __declspec(dllexport) void SendExt(MyThread* thread,int len, char *p);

#ifdef __cplusplus
}
#endif

#endif // WRAPPERTCPCLIENT_H
