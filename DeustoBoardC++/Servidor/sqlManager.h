#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

extern "C" {
#include "sqlite3.h"
}

#include "usuario.h"

sqlite3* abrirDB();
int inicioSesion(char dni[], char contrasena[], Usuario& u);
int anadirUsuario(Usuario u);

#endif