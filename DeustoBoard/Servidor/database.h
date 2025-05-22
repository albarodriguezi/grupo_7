#ifndef DATABASE_H_
#define DATABASE_H_
#include "usuario.h"
#include "servidorPruebaPartida.h"
#include "torneo.h"

void csvToDatabaseUsuario();
void csvToDatabasePartida();
void csvToDatabaseParticipa();
void csvToDatabaseTorneo();
void csvToDatabaseRegistrado();

void dibujoPerfil();

void deleteDB();

void createDB();

Usuario* getListaUsuario();
Partida* getListaPartida();
Torneo* getListaTorneo();

Usuario getUsuario(char * email);
Partida getPartida(char * codigo);

void saveUsuario(Usuario u);
void savePartida(Partida p);

int lineasFichero(char* f);

Partida* getListaPartidaJuego(char* juego);
int getTamanoListaPartida(char* juego);
int getTamanoListaTorneo();
int getTamanoListaUsuario();
#endif