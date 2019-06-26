#ifndef WRAPPERTCPCLIENT_H
#define WRAPPERTCPCLIENT_H

#define INTEGRACAO

#include <QCoreApplication>
#include <QTime>
#include "integracao.h"

#include "tcpclient.h"

extern QThread *thread;
extern TcpClient *client;
extern QCoreApplication *appMain;
extern Integracao *integracao;

#ifdef __cplusplus
extern "C" {
#endif

extern __declspec(dllexport) TcpClient* CreateTcpClient(int porta, char *servidor);

extern __declspec(dllexport) int InitClient(int porta, char *servidor);

extern __declspec(dllexport) void FinalizaClient();

extern __declspec(dllexport) void EnviaPacote(int len, char *p);

extern __declspec(dllexport) void SetOnConectado(void onConectado());

#ifdef __cplusplus
}
#endif

#endif // WRAPPERTCPCLIENT_H
