#ifndef DATABASE_H_
#define DATABASE_H_
#include "usuario.h"

void csvToDatabaseUsuario();

void dibujoPerfil();

void deleteDB();

void createDB();

Usuario* getListaUsuario();

Usuario getUsuario(char * email);

int lineasFichero(char* f);

#endif