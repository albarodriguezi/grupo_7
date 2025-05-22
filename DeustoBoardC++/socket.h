#ifndef SOCKET_H_
#define SOCKET_H_

#include <winsock2.h>

int setUpSocket(SOCKET* s);

char* recibirMensaje(char *recvBuff, SOCKET comm_socket);

void enviarMensaje(char *recvBuff, char *sendBuff, const char *mensaje, SOCKET comm_socket);

int contarSlash(const char* code);

#endif