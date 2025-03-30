#ifndef PARTIDA_H_
#define PARTIDA_H_
#include <stdbool.h>
#include "usuario.h"
#include <time.h>



typedef struct{
    int codigo[4];
    int resultado;
    bool activa;
    struct tm fecha;
    char registroMov[100];
    bool amistosoCom;

}Partida;
/*
    
    // ejemplo de como seleccionar una fecha específica (ejemplo: 10 abril 2025)
    e.fecha.tm_mday = 10;
    e.fecha.tm_mon = 3;  // Meses van de 0 (enero) a 11 (diciembre)
    e.fecha.tm_year = 2025 - 1900;  // tm_year cuenta desde 1900
*/
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
int hayPiezaEnProximidad(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha);

int hayCapturaDisponible(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha);

int fichaEnArray(int** arrayFichas, int tamanyoArray, int filaFicha, int columnaFicha);

void partidaDamas();

#endif