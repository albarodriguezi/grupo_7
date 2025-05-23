#ifndef MENUS_H_
#define MENUS_H_

//#include "usuario.h"
#include "dibujos.h"
#include "log.h"
#include <winsock2.h>
#include "usuario.h"
#include "socket.h"

using namespace std; 

//menub de incio(registro, inicioSesion, salir)
void menuPrincipal(SOCKET* s, Log& logger);
void caseMenuPrincipal(int *opcion, SOCKET* s, Log& logger);

//menu registro
void menuRegistrarse(SOCKET* s, Log& logger);

//menuInicioSesion
void menuInicioSesion(SOCKET* s, Log& logger);

//menu pagina principal(elegir juego, partida, torneo...)
void menuPaginaPrincipal(SOCKET* s, Log& labs);
void casePaginaPrincipal(int *opcion, SOCKET* s, Log& logger);

//menu para seleccion el juego que se quiere jugar
void menuModoJuego(SOCKET* s, Log& logger);
void caseModoJuego(int *opcion, SOCKET* s, Log& logger);


//menu para ver las partidad disponibles
void menuPartidasDisponibles(SOCKET* s, Log& logger);

void menuUnirsePartida(SOCKET* s, Log& logger);


//menu para ver las torneos disponibles
void menuTorneosDisponibles(SOCKET* s, Log& logger);

void menuUnirseTorneo(SOCKET* s, Log& logger);

//SOCKETS

void enviarComandoRegistro(SOCKET* s, Usuario& u);

int enviarComandoIniciarSesion(SOCKET* s, char* email, char* contrasena);

void enviarComandoSalir(SOCKET *s);



#endif
