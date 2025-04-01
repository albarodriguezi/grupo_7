#include <stdio.h>
#include "partidaCuatroRaya.h"


void imprimirTableroCuatroRaya(char tablero[FILAS][COLUMNAS]) { //función que imprime el tablero actualizándolo cada vez con las fichas ya introducidas

    printf("\n   ");
    for (int col = 1; col <= COLUMNAS; col++) {
        printf(" %d  ", col);
    }
    printf("\n");

    for (int i = 0; i < FILAS; i++) {
        printf("  ");

        for (int j = 0; j < COLUMNAS; j++) {
            //printf("[%c] ", tablero[i][j]);  //fichas sin color

            if (tablero[i][j] == 'X') {
                printf("[" ANSI_COLOR_BLUE "%c" ANSI_COLOR_RESET "] ", tablero[i][j]); //color azul para las 'X'
            } else if (tablero[i][j] == 'O') {
                printf("[" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET "] ", tablero[i][j]); //color rojo para las 'O'
            } else {
                printf("[ ] ");
            }

        }
        printf("\n");
    }

    printf("  ");
    for (int col = 0; col < COLUMNAS; col++) {
        printf("----");
    }
    printf("-\n");

}

//-----------------------------


int colocarFicha(char tablero[FILAS][COLUMNAS], int columna, char ficha) {

    columna = columna - 1; //las columnas son del 1 al 7, pero los índices del array son del 0 al 6
    
    for (int fila = FILAS - 1; fila >= 0; fila--) {
        if (tablero[fila][columna] == ' ') {
            tablero[fila][columna] = ficha;
            return 1; //si la función devuelve 1 la inserción de ficha ha sido un éxito
        }
    }

    return 0; //si la función devuelve 0 la columna está llena

}



void menuCuatroRaya() {
    printf("\n***MENU CUATRO EN RAYA***\n");
    //printf("implementamos el juego aqui, en construccion por ahora.\n");
    //printf("tablero de ejemplo:\n");

    char tablero[FILAS][COLUMNAS];

    for (int i = 0; i < FILAS; i++) { //se crea el tablero para metérselo como parámetro a la función de imprimir tablero
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = ' ';
        }
    }

    //imprimirTableroCuatroRaya(tablero);

    int turno = 0; //turno 0 corresponde a jugador 1, turno 1 corresponde a jugador 2
    char fichaJugador[2] = {'X', 'O'};
    int columnaSeleccionada;

    while (1) { //bucle infinito hasta que se le indique que pare

        imprimirTableroCuatroRaya(tablero);
        printf("TURNO DE JUGADOR %d. TUS FICHAS SON LAS '%c'\n", turno + 1, fichaJugador[turno]);
        printf("Elige una columna del 1 al 7 para introducir tu ficha:\n");
        scanf("%d", &columnaSeleccionada);

        if (columnaSeleccionada < 1 || columnaSeleccionada > COLUMNAS) {
            printf("Columna fuera de rango. Por favor, elige otra:\n");
        } else if (!colocarFicha(tablero, columnaSeleccionada, fichaJugador[turno])) {
            printf("Columna completamente llena. Por favor, elige otra:\n");
        } else {
            //victoria empate etc


            if (turno == 0) { //los turnos entre jugadores se intercambian
                turno = 1;
            } else {
                turno = 0;
            }
        }
    }


}