#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "dibujos.h"
#include "principal.h"
#include "database.h"


void menuRegistrarse();
void caseRegistro(char tecla, Usuario u);
void caseInicioSesion(char tecla, Usuario u);
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
    sscanf(bufer, "%s", u.nombreUsuario);

    printf("Introduce la contrasenya de usuario: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.contrasenya);

    printf("\n");
    printf("Registro de usuario. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%c", &tecla);

    caseRegistro(tecla, u);


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

    caseInicioSesion(tecla, u);

}

void caseRegistro(char tecla, Usuario u){

    system("cls");
	switch (tecla) {
        case 'I':
            saveUsuario(u);
            menuInicioSesion();
            printf("registro correcto");
            break;
        case 'i':
            saveUsuario(u);
            menuInicioSesion();
            printf("registro correcto");
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

void caseInicioSesion(char tecla, Usuario u){

    system("cls");
	switch (tecla) {
        case 'I':
            {Usuario uDB = getUsuario(u.nombreUsuario);  // Usamos tu función existente para obtener el usuario
            
        // Verificamos si el usuario existe y si la contraseña coincide
            if (strcmp(uDB.nombreUsuario, u.nombreUsuario) == 0 && strcmp(uDB.contrasenya, u.contrasenya) == 0) {
                printf("Inicio de sesión exitoso!\n");
                elegirModoJuego();
            } else {
                printf("Email o contraseña incorrectos.\n");
                menuInicioSesion();
            }
            }break;
        case 'i':
            {Usuario uDB = getUsuario(u.email);  // Usamos tu función existente para obtener el usuario
            
            // Verificamos si el usuario existe y si la contraseña coincide
                if (strcmp(uDB.nombreUsuario, u.nombreUsuario) == 0 && strcmp(uDB.contrasenya, u.contrasenya) == 0) {
                    printf("Inicio de sesión exitoso!\n");
                    elegirModoJuego();
                } else {
                    printf("Email o contraseña incorrectos.\n");
                    menuInicioSesion();
                }
            }break;
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