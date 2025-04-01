#include <stdio.h>
#include "partidaCuatroRaya.h"
#include "principal.h"
#include <string.h>

int fichasGanadoras[4][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}; //por ahora no las hay, por eso -1


void imprimirTableroCuatroRaya(char tablero[FILAS][COLUMNAS]) { //función que imprime el tablero actualizándolo cada vez con las fichas ya introducidas
    
    printf("\n   ");
    for (int col = 1; col <= COLUMNAS; col++) {
        printf(" %d  ", col);
    }
    printf("\n");


    for (int i = 0; i < FILAS; i++) {
        printf("  ");
        for (int j = 0; j < COLUMNAS; j++) {
            int esGanadora = 0; //las fichas que sirvan para la victoria se pintarán de amarillo
            for (int k = 0; k < 4; k++) {
                if (fichasGanadoras[k][0] == i && fichasGanadoras[k][1] == j) {
                    esGanadora = 1;
                    break;
                }
            }

            if (esGanadora) {
                printf("[" ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET "] ", tablero[i][j]); //si la ficha es de las ganadoras es amarilla
            } else if (tablero[i][j] == 'X') {
                printf("[" ANSI_COLOR_BLUE "%c" ANSI_COLOR_RESET "] ", tablero[i][j]); //si es 'X' es azul
            } else if (tablero[i][j] == 'O') {
                printf("[" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET "] ", tablero[i][j]); //si es 'O' es roja
            } else {
                printf("[ ] ");
            }
        }
        printf("\n");
    }

    printf("  ");
    for (int col = 0; col < COLUMNAS; col++) {
        printf("----");
    }
    printf("-\n");
}




int colocarFicha(char tablero[FILAS][COLUMNAS], int columna, char ficha) { //función que coloca fichas en el tablero

    columna = columna - 1; //las columnas son del 1 al 7, pero los índices del array son del 0 al 6
    
    for (int fila = FILAS - 1; fila >= 0; fila--) {
        if (tablero[fila][columna] == ' ') {
            tablero[fila][columna] = ficha;
            return 1; //si la función devuelve 1 la inserción de ficha ha sido un éxito
        }
    }

    return 0; //si la función devuelve 0 la columna está llena

}


int hayGanador(char tablero[FILAS][COLUMNAS], char ficha) { //función que comprueba si hay un ganador. será llamada tras cada inserción de ficha

    //comprobación de victoria horizontal:
    for (int fila = 0; fila < FILAS; fila++) {

        for (int col = 0; col <= COLUMNAS - 4; col++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila][col + 1] == ficha 
            && tablero[fila][col + 2] == ficha 
            && tablero[fila][col + 3] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila;
                fichasGanadoras[i][1] = col + i;
                }

                return 1; //victoria encontrada
            }
        }

    }

    //comprobación de victoria vertical:
    for (int col = 0; col < COLUMNAS; col++) {

        for (int fila = 0; fila <= FILAS - 4; fila++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila + 1][col] == ficha 
            && tablero[fila + 2][col] == ficha 
            && tablero[fila + 3][col] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila + i;
                fichasGanadoras[i][1] = col;
                }

                return 1; //victoria encontrada
            }
        }

    }

    //comprobación de victoria diagonal hacia abajo:
    for (int fila = 0; fila <= FILAS - 4; fila++) { //-4 necesario para no mirar en índices incorrectos

        for (int col = 0; col <= COLUMNAS - 4; col++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila + 1][col + 1] == ficha 
            && tablero[fila + 2][col + 2] == ficha 
            && tablero[fila + 3][col + 3] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila + i;
                fichasGanadoras[i][1] = col + i;
                }

                return 1; //victoria encontrada
            }
        }

    }

    //comprobación de victoria diagonal hacia arriba:
    for (int fila = 3; fila < FILAS; fila++) { // el fila = 3 necesario para no mirar en índices incorrectos

        for (int col = 0; col <= COLUMNAS - 4; col++) { //el -4 es necesario para no salir del rango del tablero a la hora de detectar victorias
            if (tablero[fila][col] == ficha 
            && tablero[fila - 1][col + 1] == ficha 
            && tablero[fila - 2][col + 2] == ficha 
            && tablero[fila - 3][col + 3] == ficha) {

                for (int i = 0; i < 4; i++) { //se almacena como ficha ganadora
                fichasGanadoras[i][0] = fila - i;
                fichasGanadoras[i][1] = col + i;
                }

                return 1; //victoria encontrada
            }
        }

    }

    return 0; //todavía no hay victoria

}

int tableroLleno(char tablero[FILAS][COLUMNAS]) { //función que detecta si el tablero está completo. se traduce a empate entre los jugadores
    for (int col = 0; col < COLUMNAS; col++) {
        if (tablero[0][col] == ' ') {
            return 0; //0 significa que aún hay espacio en esta columna
        }
    }
    return 1; //1 significa que todas las columnas están llenas
}



void menuCuatroRaya(Partida* partida) { //función principal del cuatro en raya, que se encarga de actualizar el programa hasta que uno de los dos jugadores gane o haya empate
    printf("\n***MENU CUATRO EN RAYA***\n");
    //printf("implementamos el juego aqui, en construccion por ahora.\n");
    //printf("tablero de ejemplo:\n");

    char tablero[FILAS][COLUMNAS];

    for (int i = 0; i < FILAS; i++) { //se crea el tablero para metérselo como parámetro a la función de imprimir tablero
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = ' ';
        }
    }

    //imprimirTableroCuatroRaya(tablero);

    for (int i = 0; i < 4; i++) { //las fichas ganadoras se reinician. necesario para que no sean amarillas en caso de jugar dos partidas seguidas
    fichasGanadoras[i][0] = -1;
    fichasGanadoras[i][1] = -1;
    }
    
    FILE* fichero = crearCSVPartida("partidas.csv"); //cuál es el fichero
    strcpy(partida->juego, "CuatroRaya");
    almacenarDatosPartida(partida->codigo, 0, partida->juego, partida->fecha, partida->codigotorneo, fichero);
    

    int turno = 0; //turno 0 corresponde a jugador 1, turno 1 corresponde a jugador 2
    char fichaJugador[2] = {'X', 'O'};
    int columnaSeleccionada;

    while (1) { //bucle infinito hasta que se le indique que pare

        imprimirTableroCuatroRaya(tablero);
        printf("TURNO DE JUGADOR %d. TUS FICHAS SON LAS '%c'\n", turno + 1, fichaJugador[turno]);
        printf("Elige una columna del 1 al 7 para introducir tu ficha:\n");
        scanf("%d", &columnaSeleccionada);

        if (columnaSeleccionada < 1 || columnaSeleccionada > COLUMNAS) {
            printf("Columna fuera de rango. Por favor, elige otra:\n");
        } else if (!colocarFicha(tablero, columnaSeleccionada, fichaJugador[turno])) {
            printf("Columna completamente llena. Por favor, elige otra:\n");
        } else {
            //victoria, empate y cambiar de turnos

            if (hayGanador(tablero, fichaJugador[turno])) {
                imprimirTableroCuatroRaya(tablero);

                printf("JUGADOR %d HA GANADO. \n", turno + 1);
            
                partida->resultado = turno + 1; //jugador 1 = 1, jugador 2 = 2
                almacenarDatosPartida(partida->codigo, partida->resultado, partida->juego, partida->fecha, partida->codigotorneo, fichero); //se guardan los datos

                char opcion[5]; //la respuesta del jugador
                do {
                    printf("Quieres volver al menu principal? En caso negativo saldras del sistema. S/N\n");
                    fflush(stdin);
                    fgets(opcion, sizeof(opcion), stdin);
                } while (opcion[0] != 's' && opcion[0] != 'S' && opcion[0] != 'n' && opcion[0] != 'N');
                fclose(fichero);
                if (opcion[0] == 's' || opcion[0] == 'S') {
                    paginaPrincipal();
                } else {
                    printf("Saliendo del sistema.\n");
                }
                
                break;

            }

            if (tableroLleno(tablero)) {
                imprimirTableroCuatroRaya(tablero);
                printf("EMPATE. El tablero esta lleno\n");
                
                partida->resultado = 0; //empate = 0
                almacenarDatosPartida(partida->codigo, partida->resultado, partida->juego, partida->fecha, partida->codigotorneo, fichero); //se almacenan los datos
                fclose(fichero);
                char opcion[5]; //la respuesta del jugador
                do {
                    printf("Quieres volver al menu principal? En caso negativo saldras del sistema. S/N\n");
                    fflush(stdin);
                    fgets(opcion, sizeof(opcion), stdin);
                } while (opcion[0] != 's' && opcion[0] != 'S' && opcion[0] != 'n' && opcion[0] != 'N');
                
                if (opcion[0] == 's' || opcion[0] == 'S') {
                    paginaPrincipal();
                } else {
                    printf("Saliendo del sistema.\n");
                }
                
                break;

            }
        

            if (turno == 0) { //los turnos entre jugadores se intercambian
                turno = 1;
            } else {
                turno = 0;
            }
               
        }   
    }
}