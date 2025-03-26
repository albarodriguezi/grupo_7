#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partida.h"
#include "dibujos.h"
#include "principal.h"
#include <stdbool.h>

void metodoEjemploPartida(){
    printf("estas en la pagina de las partidas");
}

Partida crearPartida(){
    Partida partida;
    char str[5];
    int opcionRival;
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
        
        return partida;
    case 2:
        if(buscarAmigo()){
            
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
    char str[20];
    char nombreUsuario[20];
    printf("Introduce su nombre de usuario: ");
    fgets(str, 20, stdin);
    sscanf(str, "%s", nombreUsuario);
    fflush(stdin);
    if(1/*si el nombre de usuario está en la lista de jugadores, se verifica y va a la partida*/){
    
        return true;
    }else{
        printf("El jugador <%s> no se ha encontrado", nombreUsuario);
        return false;
    }

}
