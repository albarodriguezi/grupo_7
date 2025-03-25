#include <stdio.h>
#include "usuario.h"

typedef struct{
    char email;
    char usuario;
    char contrasenya;
}Usuario;

void menuRegistrarse(){

    printf("*********************************************************************");
    printf("************************REGISTRO USUARIO*****************************");
    printf("*********************************************************************");

    printf("Introduce el nombre: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.nombre);

	printf("Introduce el apellido: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.apellido);

}

void menuInicioSesion(){

}