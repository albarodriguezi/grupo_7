#include <stdio.h>
#include "partidaCuatroRaya.h"

#define FILAS 6
#define COLUMNAS 7


void imprimirTableroCuatroRaya(char tablero[FILAS][COLUMNAS]) {

    printf("\n   ");
    for (int col = 1; col <= COLUMNAS; col++) {
        printf(" %d  ", col);
    }
    printf("\n");


    for (int i = 0; i < FILAS; i++) {
        printf("  ");
        for (int j = 0; j < COLUMNAS; j++) {
            printf("[%c] ", tablero[i][j]);
        }
        printf("\n");
    }


    printf("  ");
    for (int col = 0; col < COLUMNAS; col++) {
        printf("----");
    }
    printf("-\n");

}


void menuCuatroRaya() {
    printf("\n***MENU CUATRO EN RAYA***\n");
    printf("implementamos el juego aqui, en construccion por ahora.\n");
    printf("tablero de ejemplo:\n");

    char tablero[FILAS][COLUMNAS];

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = ' ';
        }
    }

    imprimirTableroCuatroRaya(tablero);

}