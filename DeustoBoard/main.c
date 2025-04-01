#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menuPrincipal.h"
#include "dibujos.h"
#include "usuario.h"
#include "database.h"

int main(void) {
	system("cls");
	//deleteDB();
	createDB();
	csvToDatabaseUsuario();
	csvToDatabasePartida();
	csvToDatabaseParticipa();
	csvToDatabaseTorneo();
	csvToDatabaseRegistrado();
	menuPrincipal();


	return 0;
}