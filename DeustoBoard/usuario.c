#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "dibujos.h"
#include "principal.h"


void menuRegistrarse();
void caseRegistro(char tecla);
void caseInicioSesion(char tecla);
void menuInicioSesion();

void menuRegistrarse(){

    Usuario u;

    char bufer[50];

    char tecla; 

    dibujoPerfil();
    printf("\n");

    printf("********************************************************************* \n");
    printf("************************REGISTRO USUARIO***************************** \n");
    printf("********************************************************************* \n");

    printf("Introduce el email: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.email);


	printf("Introduce el nombre de usuario: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.contrasenya);

    printf("Introduce la contrasenya de usuario: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%c", &tecla);

    printf("\n");
    printf("Registro de usuario. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%c", &tecla);

    caseRegistro(tecla);


}



void menuInicioSesion(){

    Usuario u;

    char bufer[50];

    char tecla;

    dibujoPerfil();
    printf("\n");

    printf("********************************************************************* \n");
    printf("************************INICIO DE SESION***************************** \n");
    printf("********************************************************************* \n");

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.nombreUsuario);

    printf("Introduce la contrasenya de usuario: ");
	fflush(stdout);
	fflush(stdin);
    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.contrasenya);

    printf("\n");
    printf("Inicio de sesion. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%c", &tecla);

    caseInicioSesion(tecla);

}

void caseRegistro(char tecla){

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
            menuRegistrarse(tecla);
            break;
    }
}

void caseInicioSesion(char tecla){

    system("cls");
	switch (tecla) {
        case 'I':
            elegirModoJuego();
            break;
        case 'i':
            elegirModoJuego();
            break;
        case 'E':
            printf("saliendo de DEUSTO BOARD");
            break;
        case 'e':
            printf("saliendo de DEUSTO BOARD");
            break;
        default:
            printf("Tecla inválida. Inténtalo de nuevo.\n");
            menuInicioSesion(tecla);
            break;
    }
}