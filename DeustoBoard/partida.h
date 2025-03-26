#ifndef PARTIDA_H_
#define PARTIDA_H_
#include <stdbool.h>

typedef struct{
    int codigo[4];
}Partida;

Partida crearPartida();
bool buscarAmigo();
void metodoEjemploPartida();

#endif