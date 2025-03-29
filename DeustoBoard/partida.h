#ifndef PARTIDA_H_
#define PARTIDA_H_
#include <stdbool.h>
#include "usuario.h"



typedef struct{
    int codigo[4];
}Partida;

Partida crearPartida();
bool buscarAmigo();
void metodoEjemploPartida();
int seleccionarJugadorAleatorio();

typedef struct{
    int array[8][8];
}Tablero8x8;

void metodoEjemploPartida();

Tablero8x8 crearTableroDamas();

void imprimirNumerosTablero(Tablero8x8 tablero);

void imprimirTableroDamas(Tablero8x8 tablero);

void imprimirTableroDamasconSeleccion(Tablero8x8 tablero, int fila, int columna);

void turnoJugador(Tablero8x8* tableroDamas, char str[4], int movimiento, int numJugador);
//void turnoJugador2(Tablero8x8* tableroDamas, char str[4], int movimiento);
int hayPiezaBandoOpuesto(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha);

int hayCapturaDisponible(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha);

int fichaEnArray(int** arrayFichas, int tamanyoArray, int filaFicha, int columnaFicha);

void partidaDamas();

#endif