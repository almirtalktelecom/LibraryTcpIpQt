#ifndef WRAPPERTCPCLIENT_H
#define WRAPPERTCPCLIENT_H

#include <QCoreApplication>

#include "tcpclient.h"
#include "mythread.h"

#ifdef __cplusplus
extern "C" {
#endif

extern __declspec(dllexport) TcpClient* CreateTcpClient();

extern __declspec(dllexport) MyThread* CreateMyThread();

extern __declspec(dllexport) void InitClient(MyThread* thread);

extern __declspec(dllexport) void SendExt(MyThread* thread,int len, char *p);

#ifdef __cplusplus
}
#endif

#endif // WRAPPERTCPCLIENT_H
