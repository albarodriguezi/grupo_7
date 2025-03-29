#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partida.h"
#include "dibujos.h"
#include "principal.h"
#include <stdbool.h>
#include "usuario.h"
#include "database.h"
#include "sqlite3.h"
#include <time.h>

void metodoEjemploPartida(){
    printf("estas en la pagina de las partidas");
}
Usuario *listaUsuario;

Partida crearPartida(){
    createDB();
    csvToDatabaseUsuario();
    Partida partida;
    char str[5];
    int opcionRival;
    srand(time(NULL));
    for(int i = 0; i < 4; i++){
        partida.codigo[i] = rand() % 10;
    }

    do{
    printf("El codigo de la partida es: %d%d%d%d\n", partida.codigo[0], partida.codigo[1], partida.codigo[2], partida.codigo[3]);
    printf("\n1.Jugar con un jugador aleatorio\n2.Jugar con un amigo\n3.Salir\n");
    fflush(stdin);
    fgets(str, 5, stdin);
    sscanf(str, "%d", &opcionRival);
    fflush(stdin);

    
    switch (opcionRival)
    {
    case 1: 
        //seleccionar jugador aleatorio del csv
        listaUsuario = getListaUsuario();
        int n = seleccionarJugadorAleatorio();
        printf("Jugaras contra el jugador: <%s>", listaUsuario[n].nombreUsuario);
        //aqui tiene que ir a la partida para jugar contra uno aleatorio
        free(listaUsuario);
        return partida;
    case 2:
        if(buscarAmigo()){
            //aqui empezaria la partida con un amigo, (guardar nombre de usuario para los resultados de la partida3)
            return partida;
        }else{
            break;
        }
        
    case 3:
        paginaPrincipal();
        
        break;
    }
    }while(1);

return partida;
    
}

bool buscarAmigo(){
    createDB();
    csvToDatabaseUsuario();
    listaUsuario = getListaUsuario();
    char str[20];
    char nombreUsuario[20];
    int numLineas = lineasFichero("users.csv");


    while(1) {
        printf("Introduce su nombre de usuario: ");
        fgets(str, sizeof(str), stdin);
        sscanf(str, "%s", nombreUsuario);
        fflush(stdin);

        // Eliminar el salto de línea si está presente
        nombreUsuario[strcspn(nombreUsuario, "\n")] = 0;

        //si el nombre de usuario existe
        for(int i = 0; i < numLineas; i++) {
            if (strcmp(listaUsuario[i].nombreUsuario, nombreUsuario) == 0) {
                printf("Jugador <%s> encontrado. Iniciando partida...\n", nombreUsuario);
                free(listaUsuario);
                return true;
            }
        }

        //pedir reintento o salida
        printf("El jugador <%s> no se ha encontrado.\n", nombreUsuario);
        char tecla;
        printf("\nIntenta otra vez. Pulsa 's' si quieres buscar jugador. Pulsa 'n' si quieres salir: \n");
        fgets(str, sizeof(str), stdin);
        sscanf(str, "%c", &tecla);
        fflush(stdin);

        if(tecla == 'N' || tecla == 'n') {
            free(listaUsuario); 
            return false;
        }
    }
}

int seleccionarJugadorAleatorio(){
    
    srand(time(NULL));

    int tamanyo = lineasFichero("users.csv");
    int posicionAleatoria = rand() % tamanyo;

    return posicionAleatoria;
}
