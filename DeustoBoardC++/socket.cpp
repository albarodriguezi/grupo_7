#include "socket.h"
#include "fichero.h"

#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <iomanip>

using namespace std;

int setUpSocket(SOCKET* s){
	WSADATA wsaData;
	struct sockaddr_in server;

	cout << "Iniciando libreria socket\n" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << " No se ha podido inicializar la libreria socket. Codigo de error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Libreria socket inicializada" << endl;

	//SOCKET creation
	if ((*s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "No se ha podido crear el socket. Codigo de error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	cout << "Socket creado correctamente" << endl;

	//Fichero f;
	//f.leerConfig();
	/*
	server.sin_addr.s_addr = inet_addr(f.getIp());
	server.sin_family = AF_INET;
	server.sin_port = htons(f.getPuerto());
	*/
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(6000);

	//CONNECT to remote server
	if (connect(*s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Error al conectar el socket. Codigo de error: " << WSAGetLastError() << endl;
		closesocket(*s);
		WSACleanup();
		return -1;
	}

	cout << "Conexion establecida con: " << inet_ntoa(server.sin_addr) << " (" << ntohs(server.sin_port) << ")" << endl;
	return 0;
}

void enviarMensaje(char* recvBuff, char* sendBuff, const char* mensaje, SOCKET comm_socket) {
    std::cout << "Sending reply... \n";
    strcpy(sendBuff, "ACK -> ");
    std::cout << "About to copy receive buff...\n";
    strcat(sendBuff, recvBuff);
    std::cout << "About to copy the message...\n";
    strcat(sendBuff, "\n");
    strcat(sendBuff, mensaje);
    std::cout << "Almost sent...\n";
    send(comm_socket, sendBuff, 1024, 0);
    std::cout << "Sent!\n";
}

char* recibirMensaje(char* recvBuff, SOCKET comm_socket) {
    while (true) {
        int bytes = recv(comm_socket, recvBuff, 512, 0);
        if (bytes > 0) {
            std::cout << "Recibiendo mensaje... \n";
            std::cout << "Datos recibidos: " << recvBuff << " \n";
            break;
        }
    }
    return recvBuff;
}

int contarSlash(const char* code) {
    int count = 0;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '/') {
            count++;
        }
    }
    return count;
}