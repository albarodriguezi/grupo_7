#ifndef PARTIDA_CUATRORAYA_H
#define PARTIDA_CUATRORAYA_H

#define FILAS 6 //número total de filas en el tablero
#define COLUMNAS 7 //número total de columnas en el tablero

#define ANSI_COLOR_RED     "\x1b[31m" //color de ficha roja
#define ANSI_COLOR_BLUE    "\x1b[34m" //color de ficha azul
#define ANSI_COLOR_YELLOW "\x1b[33m" //color de ficha amarilla (ganadora)
#define ANSI_COLOR_RESET   "\x1b[0m" //para reiniciar el color del terminal

void menuCuatroRaya(void);

void imprimirTableroCuatroRaya(char tablero[FILAS][COLUMNAS]);

int colocarFicha(char tablero[FILAS][COLUMNAS], int columna, char ficha);

int hayGanador(char tablero[FILAS][COLUMNAS], char ficha);

int tableroLleno(char tablero[FILAS][COLUMNAS]);

#endif
