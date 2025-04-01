#ifndef DATABASE_H_
#define DATABASE_H_
#include "usuario.h"
#include "partida.h"

void csvToDatabaseUsuario();
void csvToDatabasePartida();
void csvToDatabaseParticipa();

void dibujoPerfil();

void deleteDB();

void createDB();

Usuario* getListaUsuario();
Partida* getListaPartida();

Usuario getUsuario(char * email);
Partida getPartida(char * codigo);

void saveUsuario(Usuario u);
void savePartida(Partida p);

int lineasFichero(char* f);

#endif