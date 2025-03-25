#include <stdio.h>
#include "usuario.h"

void menuRegistrarse(Usuario u){

    printf("*********************************************************************");
    printf("************************REGISTRO USUARIO*****************************");
    printf("*********************************************************************");

    printf("Introduce el email: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.email);

	printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.nombreUsuario);

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.contrasenya);

}

void menuInicioSesion(){

}