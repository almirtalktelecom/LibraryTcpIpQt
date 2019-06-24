#ifndef WRAPPERTCPCLIENT_H
#define WRAPPERTCPCLIENT_H

#include <QCoreApplication>

#include "tcpclient.h"

extern QThread *thread;
extern TcpClient *client;
extern QCoreApplication *appMain;

#ifdef __cplusplus
extern "C" {
#endif

extern __declspec(dllexport) TcpClient* CreateTcpClient(int porta, char *servidor);

extern __declspec(dllexport) int InitClientExt(int porta, char *servidor);

extern __declspec(dllexport) void FinalizaClientExt();

extern __declspec(dllexport) void SetPacote(int len, char *p);

#ifdef __cplusplus
}
#endif

#endif // WRAPPERTCPCLIENT_H
