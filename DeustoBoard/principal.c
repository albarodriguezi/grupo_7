#include <stdio.h>
#include <stdlib.h>
#include "menuPrincipal.h"
#include "principal.h"
#include "partida.h"
#include "torneo.h"
#include "dibujos.h"

void paginaPrincipal();
void casePaginaPrincipal(int opcion);

void paginaPrincipal() {

    int opcion;

    dibujoPaginaPrincipal();
    printf("**********************************************\n");
    printf("********** ELIGA FORMA DE JUEGO **************\n");
    printf("*********************************************\n");

    printf("1. Crear partida \n");
    printf("2. Unirse partida \n");
    printf("3. Unirse Torneo \n");
    printf("4. Ver partidas disponibles \n");
    printf("5. Ver torneos disponibles \n");
    printf("6. Exit \n");

    printf("Introduzca una opcion: \n");

    fflush(stdout);
    fflush(stdin);
    scanf("%d", &opcion);

    casePaginaPrincipal(opcion);
}

void casePaginaPrincipal(int opcion) {
    system("cls");
    switch (opcion) {
        case 1:
            metodoEjemploPartida();
            break;
        case 2:
            metodoEjemploPartida();
            break;
        case 3:
            metodoEjemploTorneo();
            break;
        case 4:
            metodoEjemploPartida();
            break;
        case 5:
            metodoEjemploTorneo();
            break;  
        case 6:
            printf("Saliendo de DEUSTO BOARD\n");
            break;
        default:
            printf("El digito introducido no corresponde a ninguno de los anteriores\n");
            paginaPrincipal();
            break;
    }
}
