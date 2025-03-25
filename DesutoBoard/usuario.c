#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "dibujos.h"

void menuRegistrarse();
void caseUsuario(char tecla);
void menuInicioSesion();

void menuRegistrarse(){

    Usuario u;

    char tecla; 

    dibujoPerfil();
    printf("\n");

    printf("********************************************************************* \n");
    printf("************************REGISTRO USUARIO***************************** \n");
    printf("********************************************************************* \n");

    printf("Introduce el email: ");
	fflush(stdout);
	fflush(stdin);
    fgets(u.email, sizeof(u.email), stdin);

	printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
    fgets(u.nombreUsuario, sizeof(u.nombreUsuario), stdin);

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	fgets(u.contrasenya, sizeof(u.contrasenya), stdin);

    printf("\n");
    printf("Inicio de sesion. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);
	scanf("%c",&tecla);

    caseUsuario(tecla);


}



void menuInicioSesion(){

    Usuario u;
    char tecla;

    dibujoPerfil();
    printf("\n");

    printf("********************************************************************* \n");
    printf("************************INICIO DE SESION***************************** \n");
    printf("********************************************************************* \n");

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
    fgets(u.nombreUsuario, sizeof(u.nombreUsuario), stdin);

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
    fgets(u.contrasenya, sizeof(u.contrasenya), stdin);

    printf("\n");
    printf("Inicio de sesion. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);
	scanf("%c",&tecla);



    caseUsuario(tecla);

}

void caseUsuario(char tecla){

    system("cls");
	switch (tecla) {
        case 'I':
            menuInicioSesion();
            break;
        case 'i':
            menuInicioSesion();
            break;
        case 'E':
            printf("saliendo de DEUSTO BOARD");
            break;
        case 'e':
            printf("saliendo de DEUSTO BOARD");
            break;
        default:
            printf("Tecla inválida. Inténtalo de nuevo.\n");
            caseUsuario(tecla);
            break;
    }
}