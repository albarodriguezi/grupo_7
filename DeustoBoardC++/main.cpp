#include "fichero.h"
#include "log.h"
#include "socket.h"
#include "menus.h"
#include <winsock2.h>


int main() {
	//CONFIGURAR LOGGER
	Fichero f;
	f.leerConfig();
	Log logger(f.getLog());

	SOCKET s;
	setUpSocket(&s);
	logger.anadirLog("Socket configurado correctamente");

	menuPrincipal(&s, logger);

	return 0;
}