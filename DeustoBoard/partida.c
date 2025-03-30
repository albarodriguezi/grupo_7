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
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void metodoEjemploPartida(){
    //printf("\nestas en la pagina de las partidas");
    //Comentar esta siguiente función para impedir que comience una partida a las damas
    partidaDamas();
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
        metodoEjemploPartida();
        free(listaUsuario);
        return partida;
    case 2:
        if(buscarAmigo()){
            //aqui empezaria la partida con un amigo, (guardar nombre de usuario para los resultados de la partida3)
            metodoEjemploPartida();
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
void partidaDamas(){
    printf("\nIniciando partida a las Damas...\n");
    Tablero8x8 tableroDamas = crearTableroDamas();
    imprimirTableroDamas(tableroDamas);
    int isGameOver = 0;
    int numDamasN = 12;
    int numDamasB = 12;
    int ficha[2];
    //Necesito este string para recoger el input
    char str[4];
    int movimiento;
    //Bucle del juego
    do
    {
        turnoJugador(&tableroDamas, &str[4], movimiento,2);
        turnoJugador(&tableroDamas, &str[4], movimiento,1);

        
    } while (isGameOver != 1);
    
}

Tablero8x8 crearTableroDamas(){
    //Número 1-----Dama blanca
    //Número 2-----Reina blanca
    //Número 3-----Dama negra
    //Número 4-----Reina negra
    Tablero8x8 tableroDamas;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i<3)
            {
                if ((j+i+1)%2 == 0)
                {
                    tableroDamas.array[i][j] = 3;
                }else{
                    tableroDamas.array[i][j] = 0;
                }
                
            }else if(i>4){
                if ((j+i+1)%2 == 0)
                {
                    tableroDamas.array[i][j] = 1;
                }else{
                    tableroDamas.array[i][j] = 0;
                }
            }else{
                tableroDamas.array[i][j] = 0;
            }
            
        }
        
    }
    return tableroDamas;
    
}

void imprimirNumerosTablero(Tablero8x8 tablero){
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("[%i]",tablero.array[i][j]);
        }
        printf("\n");
    }
}
void imprimirTableroDamas(Tablero8x8 tablero){
    for (int i = 0; i < 8; i++)
    {
        printf("%i",i+1);
        for (int j = 0; j < 8; j++)
        {
            switch (tablero.array[i][j])
            {
            case 0:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"[ ]"ANSI_COLOR_RESET);
            }else{
                printf("[ ]");
            }
                break;
            case 1:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"["ANSI_COLOR_RESET);
                printf("B");
                printf(ANSI_COLOR_RED"]"ANSI_COLOR_RESET);
            }else{
                printf("[B]");
            }
            break;
            case 2:
            printf("[DB]");
            break;
            case 3:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"[N]"ANSI_COLOR_RESET);
            }else{
                printf("[");
                printf(ANSI_COLOR_RED"N"ANSI_COLOR_RESET);
            printf("]");
            }
            break;
            case 4:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"[DN]"ANSI_COLOR_RESET);
            }else{
                printf("[");
                printf(ANSI_COLOR_RED"DN"ANSI_COLOR_RESET);
            printf("]");
            }
            break;
            default:
                break;
            }
        }
        printf("\n");
    }
    printf("  1  2  3  4  5  6  7  8\n");
}
void imprimirTableroDamasconSeleccion(Tablero8x8 tablero, int fila, int columna){
    int isSelected = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {if (i== fila && j == columna)
        {
            isSelected = 1;
            printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);
        }
        
            switch (tablero.array[i][j])
            {
            case 0:
                printf("[ ]");
                break;
            case 1:
            printf("[B]");
            break;
            case 2:
            printf("[DB]");
            break;
            case 3:
            printf("[N]");
            break;
            case 4:
            printf("[DN]");
            break;
            default:
                break;
            }
            if (isSelected == 1)
            {
                printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);
                isSelected = 0;
            }
            
        }
        printf("\n");
    }
}
void turnoJugador(Tablero8x8* tableroDamas, char str[4], int movimiento, int numJugador){
    int ficha[2];
    int dama;
    int damaReina;
    int direccion;
    if (numJugador == 1)
    {
        dama = 1;
        damaReina = 2;
        direccion = 1;
    }else{
        dama = 3;
        damaReina = 4;
        direccion = -1;   
    }
    int contadorPosiblesCapturas = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tableroDamas->array[i][j] == dama || tableroDamas->array[i][j] == damaReina)
            {
                if (hayCapturaDisponible(*tableroDamas,numJugador,(tableroDamas->array[i][j] == damaReina),i,j))
                {
                    contadorPosiblesCapturas += 1;
                }
                
                
            }
            
        }
        
    }
    if (contadorPosiblesCapturas >0)
    {
        int **fichasConPosibleCaptura = (int **) malloc(sizeof(int*)*contadorPosiblesCapturas);
        contadorPosiblesCapturas = 0; 
        for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tableroDamas->array[i][j] == dama || tableroDamas->array[i][j] == damaReina)
            {
                if (hayCapturaDisponible(*tableroDamas,numJugador,(tableroDamas->array[i][j] == damaReina),i,j))
                {
                    int * fichaConPosibleCaptura = (int*)malloc(sizeof(int)*2);
                    fichaConPosibleCaptura[0] = i;
                    fichaConPosibleCaptura[1] = j;
                    fichasConPosibleCaptura[contadorPosiblesCapturas] = fichaConPosibleCaptura; 
                    contadorPosiblesCapturas += 1;   
                }
                
            }
            
        }
        
    } do
        { 
    fgets(str,4,stdin);
    printf("El jugador %i solo puede cojer una de las fichas con captura disponible",numJugador);
    fgets(str, 4, stdin);
    ficha[0] = str[0] - 49;
    ficha[1] = str[2] - 49;
    printf("%i\n",ficha[0]);
    printf("%i\n",ficha[1]);
    printf("%i,%i\n", dama, tableroDamas->array[ficha[0]][ficha[1]]);
        }while (!(str[1] == ',' && ficha[0] <9 && ficha[0] >=0 && ficha[1] <9 && ficha[1] >=0 && tableroDamas->array[ficha[0]][ficha[1]] == dama)||fichaEnArray(fichasConPosibleCaptura, contadorPosiblesCapturas,ficha[0],ficha[1]) == 0);
        if (hayPiezaEnProximidad(*tableroDamas,numJugador,0,ficha[0],ficha[1]) == 1)
        {
            tableroDamas->array[ficha[0]-direccion*2][ficha[1]-2] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            tableroDamas->array[ficha[0]-direccion][ficha[1]-1] = 0;
        }else{
            tableroDamas->array[ficha[0]-direccion*2][ficha[1]+2] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            tableroDamas->array[ficha[0]-direccion][ficha[1]+1] = 0;
        }
        imprimirTableroDamas(*tableroDamas);
        
    //Libero memoria dinámica ocupada por array
        for (int i = 0; i < contadorPosiblesCapturas; i++)
        {
            free(fichasConPosibleCaptura[i]);
            fichasConPosibleCaptura[i] = NULL;
        }
        free(fichasConPosibleCaptura);
        fichasConPosibleCaptura = NULL;
        
    }else{
    
    
    //Bucle del input del jugador (hasta que input sea válido)
        do
        {   
            labelFicha:
            fgets(str,4,stdin);
            if (numJugador == 1)
            {
                printf("Jugador %i, escoja una ficha valida (Blancas) para mover (Formato <<fila,columna>>)\n", numJugador);
            }else{
            printf("Jugador %i, escoja una ficha valida (Negras) para mover (Formato <<fila,columna>>)\n", numJugador);
            }
            fgets(str, 4, stdin);
            //printf(str);
            //Resto 49 para convertir número ASCI a numero normal (-48) y restar 1 (las casillas empiezan en 1,
            //pero los indices en 0) al mismo tiempo
            ficha[0] = str[0] - 49;
            ficha[1] = str[2] - 49;
        } while (!(str[1] == ',' && ficha[0] <9 && ficha[0] >=0 && ficha[1] <9 && ficha[1] >=0 && tableroDamas->array[ficha[0]][ficha[1]] == dama));
        imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
        fgets(str, 2, stdin);
        int isdamaReina = 0;
        labelMovimiento:
        if (tableroDamas->array[ficha[0]][ficha[1]] == damaReina)
        {
            isdamaReina = 1;
            printf("Jugador %i, escoja un movimiento (1 Adelante Izquierda, 2 Adelante Derecha, 3 Atras Izquierda, 4 Atras Derecha,5 para volver a la seleccion de ficha)\n", numJugador);
        }else{
        printf("Jugador %i, escoja un movimiento (1 Izquierda, 2 Derecha, 5 para volver a la seleccion de ficha)\n", numJugador);
        }
            fgets(str, 2, stdin);
            movimiento = str[0] - 48;
            fgets(str,2,stdin);
            if ((ficha[1] == 0 && movimiento == 1) || (ficha[1] == 7 && movimiento == 2) || (((ficha[0] == 0 && direccion == 1) || (ficha[0] == 7 && direccion == -1))&& movimiento != 5))
            {
                printf("Este movimiento sacaria la ficha del tablero (invalido)\n");
                goto labelMovimiento;
            }
            
            switch (movimiento)
            {
            case 1:
            if ( tableroDamas->array[ficha[0]-direccion][ficha[1]-1] != 0)
            {
            printf("No puedes ocupar el mismo espacio que otra ficha\n");
            goto labelMovimiento;
            }
                tableroDamas->array[ficha[0]-direccion][ficha[1]-1] = tableroDamas->array[ficha[0]][ficha[1]];
                tableroDamas->array[ficha[0]][ficha[1]] = 0;
                break;
            case 2:
            if ( tableroDamas->array[ficha[0]-direccion][ficha[1]+1] != 0)
            {
            printf("No puedes ocupar el mismo espacio que otra ficha\n");
            imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
            goto labelMovimiento;
            }
                tableroDamas->array[ficha[0]-direccion][ficha[1]+1] = tableroDamas->array[ficha[0]][ficha[1]];
                tableroDamas->array[ficha[0]][ficha[1]] = 0;
            break;
            case 3:
                if (isdamaReina)
                {
                    if ( tableroDamas->array[ficha[0]+direccion][ficha[1]-1] != 0)
                    {
                    printf("No puedes ocupar el mismo espacio que otra ficha\n");
                    imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
                    goto labelMovimiento;
                    }else{
                    tableroDamas->array[ficha[0]+direccion][ficha[1]-1] = tableroDamas->array[ficha[0]][ficha[1]];
                    tableroDamas->array[ficha[0]][ficha[1]] = 0;
                    }
                }else{
                    goto labelMovimiento;
                }
                break;
            case 4:
            if (isdamaReina)
                {
                    if ( tableroDamas->array[ficha[0]+direccion][ficha[1]+1] != 0)
                    {
                    printf("No puedes ocupar el mismo espacio que otra ficha\n");
                    imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
                    goto labelMovimiento;
                    }else{
                    tableroDamas->array[ficha[0]+direccion][ficha[1]+1] = tableroDamas->array[ficha[0]][ficha[1]];
                    tableroDamas->array[ficha[0]][ficha[1]] = 0;
                    }
                }else{
                    goto labelMovimiento;
                }
                break;
                
            case 5:
                goto labelFicha;
                break;
            default:
                goto labelMovimiento;
                break;
            }
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if(i == (numJugador-1)*7 && tableroDamas->array[i][j] == dama)
                {
                    tableroDamas->array[i][j] = damaReina;
                    printf("El jugador %i dispone de una nueva reina", numJugador);
                }
                }
                
            }
            imprimirTableroDamas(*tableroDamas);
            
            }
        
}
//Devuelve 0 si no hay piezas, 1 si la pieza opuesta está a la izquierda, 2 si está a la derecha,
//3 si la pieza es del mismo bando y está a la izquierda, 4 si es del mismo bando y está a la derecha y
//5 si la posición queda fuera del tablero.
int hayPiezaEnProximidad(Tablero8x8 tableroDamas, int bando, int isReina, int filaFicha, int columnaFicha){
    int direccion;
    if (bando == 1)
    {
        direccion = 1;
        if (filaFicha < 0 )
        {
            return 5;
        }
        
    }else{
        direccion = -1;
        if (filaFicha > 7 )
        {
            return 5;
        }
    }
        if (tableroDamas.array[filaFicha-direccion][columnaFicha-1] == 3+(direccion-1) || tableroDamas.array[filaFicha-direccion][columnaFicha-1] == 4+(direccion-1))
        {
            if ((columnaFicha-1)<0)
            {
                return 5;
            }else{
                return 1;
            }
        }if(tableroDamas.array[filaFicha-direccion][columnaFicha+1] == 3+(direccion-1) || tableroDamas.array[filaFicha-direccion][columnaFicha+1] == 4+(direccion-1)){
            if ((columnaFicha+1)>7)
            {
                return 5;
            }else{
                return 2;
            }
        }if(tableroDamas.array[filaFicha-direccion][columnaFicha-1] != 0){
            if ((columnaFicha-1)<0)
            {
                return 5;
            }else{
                return 3;
            }
        }if(tableroDamas.array[filaFicha-direccion][columnaFicha+1] != 0){
            if ((columnaFicha+1)>7)
            {
                return 5;
            }else{
                return 4;
            }
        }
        if (isReina == 1)
        {
            if (tableroDamas.array[filaFicha+direccion][columnaFicha-1] == 3+(direccion-1) || tableroDamas.array[filaFicha+direccion][columnaFicha-1] == 4+(direccion-1))
            {
                if ((columnaFicha-1)<0)
            {
                return 5;
            }else{
                return 1;
            }
            }if(tableroDamas.array[filaFicha+direccion][columnaFicha+1] == 3+(direccion-1) || tableroDamas.array[filaFicha+direccion][columnaFicha+1] == 4+(direccion-1)){
                if ((columnaFicha+1)>7)
            {
                return 5;
            }else{
                return 2;
            }
            }if(tableroDamas.array[filaFicha+direccion][columnaFicha-1] != 0){
                if ((columnaFicha-1)<0)
            {
                return 5;
            }else{
                return 3;
            }
            }if(tableroDamas.array[filaFicha+direccion][columnaFicha-1] != 0){
                if ((columnaFicha+1)>7)
            {
                return 5;
            }else{
                return 4;
            }
        }
        }
        return 0;
        
    /*}else{
        int direccion = -1;
        if (tableroDamas.array[filaFicha-direccion][columnaFicha-1] == 1 || tableroDamas.array[filaFicha-direccion][columnaFicha-1] == 2)
        {
            return 1;
        }else if(tableroDamas.array[filaFicha-direccion][columnaFicha+1] == 1 || tableroDamas.array[filaFicha-direccion][columnaFicha+1] == 2)
        {
            return 2;
        }
        if (isReina == 1)
        {
            if (tableroDamas.array[filaFicha+direccion][columnaFicha-1] == 1 || tableroDamas.array[filaFicha+direccion][columnaFicha-1] == 2)
            {
                return 1;
            }else if(tableroDamas.array[filaFicha+direccion][columnaFicha+1] == 1 || tableroDamas.array[filaFicha+direccion][columnaFicha+1] == 2){
                return 2;
            }
        }
        return 0;*/
        
}
int hayCapturaDisponible(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha){
    int direccion;
    if (bando == 1)
    {
        direccion = 1;
    }else{
        direccion = -1;
    }
    if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha) == 1)
    { 
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha) == 5)
        {
            return 0;
        }
    
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-direccion,columnaFicha-1) !=3 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-direccion,columnaFicha-1) !=1 ){
            return 1;
        }
        if (isDama)
        {
           if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+direccion,columnaFicha-1) != 3 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+direccion,columnaFicha-1) != 1){
            return 1;
        }
        }
        
    }else if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha) == 2){
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-direccion,columnaFicha+1) != 4 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-direccion,columnaFicha+1) != 2){
            return 1;
        }
        if (isDama)
        {
           if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+direccion,columnaFicha+1) != 4 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+direccion,columnaFicha+1) != 2){
            return 1;
        }
        }
    }
    return 0;
    
}
int fichaEnArray(int** arrayFichas, int tamanyoArray, int filaFicha, int columnaFicha){
    for (int i = 0; i < tamanyoArray; i++)
    {
        if((arrayFichas[i][0] == filaFicha) && (arrayFichas[i][1] == columnaFicha) ){
            return 1;
        }
    }
    return 0;
    
}
