// IMPORTANT: Winsock Library ("ws2_32") should be linked with -lws2_32

#include <stdio.h>
#include <winsock2.h>
#include "servidorPruebaPartida.h"
#include "servidorPruebaPartidaRaya.h"
#include "database.h"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


int fichasGanadoras[4][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}; 

void partidaCuatroRaya(char *sendBuff, char *recvBuff, SOCKET comm_socket,Partida* partida) {
    char mensaje[1024];
    strcpy(mensaje, "\nIniciando partida a Cuatro en Raya...\n Envia cualquier input para empezar\n");
    enviarMensaje(recvBuff, sendBuff, mensaje, comm_socket);
    char *temp = recibirMensaje(recvBuff, comm_socket);
    if (!strcmp(temp, "Bye")) {
        return;
    }

    FILE * fichero = crearCSVPartida("partidas.csv");
    char nomlog[11];
    printf("A punto de crear Log\n");
    printf(partida->codigo);
    time_t ahora;
    struct tm fecha_actual;
    time(&ahora); // obtener tiempo actual en formato time_t
    fecha_actual = *localtime(&ahora);
    sprintf(nomlog,"LOG/LOG%i%i%i.log",fecha_actual.tm_year+1900,fecha_actual.tm_mon,fecha_actual.tm_mday);
    FILE * log = fopen(nomlog, "a");
    if(log!=NULL)
    printf("\nLog creado\n");
    fprintf(log, "PartidaCuatroenRaya_%c%c%c%c\n",partida->codigo[0],partida->codigo[1],partida->codigo[2],partida->codigo[3]);
    fflush(log);
    menuCuatroRayaSocket(comm_socket,partida,log);

}



int colocarFicha(char tablero[FILAS][COLUMNAS], int columna, char ficha) { //función que coloca fichas en el tablero

    columna = columna - 1; //las columnas son del 1 al 7, pero los índices del array son del 0 al 6
    
    for (int fila = FILAS - 1; fila >= 0; fila--) {
        if (tablero[fila][columna] == ' ') {
            tablero[fila][columna] = ficha;
            return 1; //si la función devuelve 1 la inserción de ficha ha sido un éxito
        }
    }

    return 0; //si la función devuelve 0 la columna está llena

}


int hayGanador(char tablero[FILAS][COLUMNAS], char ficha) { //función que comprueba si hay un ganador. será llamada tras cada inserción de ficha

    //comprobación de victoria horizontal:
    for (int fila = 0; fila < FILAS; fila++) {

        for (int col = 0; col <= COLUMNAS - 4; col++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila][col + 1] == ficha 
            && tablero[fila][col + 2] == ficha 
            && tablero[fila][col + 3] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila;
                fichasGanadoras[i][1] = col + i;
                }

                return 1; //victoria encontrada
            }
        }

    }

    //comprobación de victoria vertical:
    for (int col = 0; col < COLUMNAS; col++) {

        for (int fila = 0; fila <= FILAS - 4; fila++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila + 1][col] == ficha 
            && tablero[fila + 2][col] == ficha 
            && tablero[fila + 3][col] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila + i;
                fichasGanadoras[i][1] = col;
                }

                return 1; //victoria encontrada
            }
        }

    }

    //comprobación de victoria diagonal hacia abajo:
    for (int fila = 0; fila <= FILAS - 4; fila++) { //-4 necesario para no mirar en índices incorrectos

        for (int col = 0; col <= COLUMNAS - 4; col++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila + 1][col + 1] == ficha 
            && tablero[fila + 2][col + 2] == ficha 
            && tablero[fila + 3][col + 3] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila + i;
                fichasGanadoras[i][1] = col + i;
                }

                return 1; //victoria encontrada
            }
        }

    }

    //comprobación de victoria diagonal hacia arriba:
    for (int fila = 3; fila < FILAS; fila++) { // el fila = 3 necesario para no mirar en índices incorrectos

        for (int col = 0; col <= COLUMNAS - 4; col++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila - 1][col + 1] == ficha 
            && tablero[fila - 2][col + 2] == ficha 
            && tablero[fila - 3][col + 3] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila - i;
                fichasGanadoras[i][1] = col + i;
                }

                return 1; //victoria encontrada
            }
        }

    }

    return 0; //todavía no hay victoria

}

int tableroLleno(char tablero[FILAS][COLUMNAS]) { //función que detecta si el tablero está completo. se traduce a empate entre los jugadores
    for (int col = 0; col < COLUMNAS; col++) {
        if (tablero[0][col] == ' ') {
            return 0; //0 significa que aún hay espacio en esta columna
        }
    }
    return 1; //1 significa que todas las columnas están llenas
}



void imprimirTableroCuatroRayaTexto(char tablero[FILAS][COLUMNAS], char* out) {
    strcat(out, "\n  ");
    for (int col = 1; col <= COLUMNAS; col++) {
        char num[4]; sprintf(num, " %d ", col);
        strcat(out, num);
    }
    strcat(out, "\n");

    for (int i = 0; i < FILAS; i++) {
        strcat(out, "  ");
        for (int j = 0; j < COLUMNAS; j++) {
            char cell[8];
            if (tablero[i][j] == ' ')
                strcpy(cell, "[ ]");
            else {
                sprintf(cell, "[%c]", tablero[i][j]);
            }
            strcat(out, cell);
        }
        strcat(out, "\n");
    }
}

void menuCuatroRayaSocket(SOCKET comm_socket, Partida* partida,FILE * log) {
    char tablero[FILAS][COLUMNAS];
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            tablero[i][j] = ' ';

    for (int i = 0; i < 4; i++) {
        fichasGanadoras[i][0] = -1;
        fichasGanadoras[i][1] = -1;
    }

    FILE* fichero = crearCSVPartida("partidas.csv");
    strcpy(partida->juego, "CuatroRaya");
    almacenarDatosPartida(partida->codigo, 0, partida->juego, partida->fecha, partida->codigotorneo, fichero);


    int turno = 0;
    char fichaJugador[2] = {'X', 'O'};
    char recvBuff[512], sendBuff[1024],mensaje[1024];
    int columnaSeleccionada;

    while (1) {
        memset(mensaje, 0, sizeof(mensaje));
        imprimirTableroCuatroRayaTexto(tablero, mensaje);
        char turnoMsg[512];
        sprintf(turnoMsg, "\nTurno del jugador %d (%c). Introduce columna (1-7):", turno + 1, fichaJugador[turno]);
        strcat(mensaje, turnoMsg);
        enviarMensaje(recvBuff,sendBuff,mensaje,comm_socket);

        recibirMensaje(recvBuff,comm_socket);
        columnaSeleccionada = atoi(recvBuff);

        if (columnaSeleccionada < 1 || columnaSeleccionada > COLUMNAS || !colocarFicha(tablero, columnaSeleccionada, fichaJugador[turno])) {
            enviarMensaje(recvBuff,sendBuff,"Movimiento invalido\n",comm_socket);
            recv(comm_socket,recvBuff,sizeof(recvBuff),0);
            continue;
        }

        fprintf(log, "Jugador %d: columna %d\n", turno + 1, columnaSeleccionada);
        fflush(log);

        if (hayGanador(tablero, fichaJugador[turno])) {
            memset(mensaje, 0, sizeof(mensaje));
            imprimirTableroCuatroRayaTexto(tablero, mensaje);
            sprintf(turnoMsg, "\n\u00a1Victoria del jugador %d!\n", turno + 1);
            strcat(mensaje, turnoMsg);
            enviarMensaje(recvBuff,sendBuff,mensaje,comm_socket);

            partida->resultado = turno + 1;
            almacenarDatosPartida(partida->codigo, partida->resultado, partida->juego, partida->fecha, partida->codigotorneo, fichero);
            
            break;
        }

        if (tableroLleno(tablero)) {
            memset(mensaje, 0, sizeof(mensaje));
            imprimirTableroCuatroRayaTexto(tablero, mensaje);
            strcat(mensaje, "\nEmpate: el tablero esta lleno.\n");
            enviarMensaje(recvBuff,sendBuff,mensaje,comm_socket);

            partida->resultado = 0;
            almacenarDatosPartida(partida->codigo, 0, partida->juego, partida->fecha, partida->codigotorneo, fichero);
            break;
        }

        turno = 1 - turno;
    }

    fclose(log);
    fclose(fichero);
}