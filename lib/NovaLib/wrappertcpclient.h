#ifndef WRAPPERTCPCLIENT_H
#define WRAPPERTCPCLIENT_H

#include <QCoreApplication>
#include <QTime>

#include "tcpclient.h"

extern QThread *thread;
extern TcpClient *client;
extern QCoreApplication *appMain;

#ifdef __cplusplus
extern "C" {
#endif

extern __declspec(dllexport) TcpClient* CreateTcpClient(int porta, char *servidor);

extern __declspec(dllexport) int InitClient(int porta, char *servidor);

extern __declspec(dllexport) void FinalizaClient();

extern __declspec(dllexport) void EnviaPacote(int len, char *p);

#ifdef __cplusplus
}
#endif

#endif // WRAPPERTCPCLIENT_H
