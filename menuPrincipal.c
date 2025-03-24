#include <stdio.h>
#include "dibujos.h"

void menuPrincipal(){

    int opcion;

    dibujoDeustoBoard();
    printf("**********************************************************\n");
    printf("*************** IDENTIFICACION USUARIO *******************\n");
    printf("**********************************************************\n");

    printf("1.Registarse\n");
    printf("2.Inicio de sesion\n");
    printf("3.Exit\n");

    printf("Introduzca una opcion: \n");

    fflush(stdout);
	fflush(stdin);
	scanf("%d",&opcion);

    caseMenuPrincipal(opcion);
    
}


void caseMenuPrinicpal(int opcion){

    system("cls");
	switch (opcion) {
        case 1:
            menuRegistrarse();
            break;
        case 2:
            menuInicioSesion();
            break;
        case 3:
            printf("saliendo de DEUSTO BOARD");
            break;
            default:
            printf("El digito introducido no corresponde a ninguno de los anteriores\n");
            menuPrincipal();
            break;
    }
}