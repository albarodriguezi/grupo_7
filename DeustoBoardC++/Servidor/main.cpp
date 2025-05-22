
#include <winsock2.h>
#include <iostream>
#include <cstring>

extern "C" {
#include "sqlite3.h"
}

#include "usuario.h"
#include "log.h"
#include "fichero.h"
#include "sqlManager.h"


using namespace std;

int main(void) {
    /*
     * CONFIGURAR LOGGER
     */
    Fichero f;
    f.leerConfig();
    Log logger(f.getLog());

    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[512], recvBuff[512];

    cout << "Iniciando libreria socket\n" << endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "No se ha podido inicializar la libreria socket. Codigo de error: %d", WSAGetLastError());
        logger.anadirLog(mensaje);
        cout << "No se ha podido inicializar la libreria socket. Codigo de error: " << WSAGetLastError() << endl;
        return -1;
    }
    logger.anadirLog("Libreria socket inicializada");
    cout << "Libreria socket inicializada" << endl;

    //SOCKET creation
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "No se ha podido crear el socket. Codigo de error: %d", WSAGetLastError());
        logger.anadirLog(mensaje);
        cout << "No se ha podido crear el socket. Codigo de error: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    logger.anadirLog("Socket creado correctamente");
    cout << "Socket creado correctamente" << endl;

    server.sin_addr.s_addr = inet_addr(f.getIp());
    server.sin_family = AF_INET;
    server.sin_port = htons(f.getPuerto());

    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error al vincular el socket. Codigo de error: %d", WSAGetLastError());
        logger.anadirLog(mensaje);
        cout << "Error al vincular el socket. Codigo de error: " << WSAGetLastError() <<endl;
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    logger.anadirLog("Bindeo realizado correctamente");
    cout << "Bindeo realizado correctamente" << endl;

    //LISTEN to incoming connections
    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error al activar modo escucha. Codigo de error: %d", WSAGetLastError());
        logger.anadirLog(mensaje);
        cout << "Error al activar modo escucha. Codigo de error: " << WSAGetLastError() <<endl;
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    //ACCEPT incoming connections
    logger.anadirLog("Esperando a conexiones del cliente...");
    cout << "Esperando a conexiones del cliente..." << endl;
    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

    if (comm_socket == INVALID_SOCKET) {
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error al aceptar conexion del cliente. Codigo de error: %d", WSAGetLastError());
        logger.anadirLog(mensaje);
        cout << "Error al aceptar conexion del cliente. Codigo de error: " << WSAGetLastError() << endl;
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    char mensaje[256];
    snprintf(mensaje, sizeof(mensaje), "Conexion entrante por: %s (%i)", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    logger.anadirLog(mensaje);
    cout << "Conexion entrante por: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

    // Closing the listening sockets
    closesocket(conn_socket);

    logger.anadirLog("Esperando comandos del cliente...");
    cout << "Esperando comandos del cliente..." << endl;

    do {
        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        logger.anadirLog("Comando recibido del cliente");

        cout << "Comando recibido: " << recvBuff << endl;

        if (strcmp(recvBuff, "COMP_INICIO_SESION") == 0) {
            // Recibir email y contraseña
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            char email[100];
            strcpy(email, recvBuff);
            
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            char contrasena[100];
            strcpy(contrasena, recvBuff);

            Usuario u;
            int resultado = inicioSesion(email, contrasena, u);

            // Enviar datos del usuario
            sprintf(sendBuff, "%s", u.getEmail());
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            sprintf(sendBuff, "%s", u.getConstrasenya());
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            
            // Enviar resultado
            sprintf(sendBuff, "%d", resultado);
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            logger.anadirLog("Respuesta de inicio de sesion enviada");
        }
        else if (strcmp(recvBuff, "COMP_REGISTRO") == 0) {
            Usuario u;
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            u.setEmail(recvBuff);

            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            u.setNombre(recvBuff);

            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            u.setContrasenya(recvBuff);

            anadirUsuario(u);
            logger.anadirLog("Usuario registrado correctamente");
            
            strcpy(sendBuff, "Usuario registrado");
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        }
        else if (strcmp(recvBuff, "SALIR") == 0) {
            logger.anadirLog("Cliente solicito cerrar conexion");
            strcpy(sendBuff, "Cerrando conexion");
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            break;
        }
        else {
            logger.anadirLog("Comando no reconocido recibido");
            strcpy(sendBuff, "Comando no reconocido");
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        }

    } while (1);

    // Cerrar conexión
    closesocket(comm_socket);
    WSACleanup();
    logger.anadirLog("Servidor cerrado correctamente");
    
    return 0;
}