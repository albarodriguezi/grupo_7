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
#include "partidaCuatroRaya.h"
#include <time.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void metodoEjemploPartida(){
    printf("\nestas en la pagina de las partidas");
    //Comentar esta siguiente función para impedir que comience una partida a las damas
    //partidaDamas();
    //printf("Ha concluido la partida\n");
}
Usuario *listaUsuario;
Partida *listaPartida;

Partida crearPartida(int tipoJuego){
    createDB();
    csvToDatabaseUsuario();
    Partida partida;
    char str[5];
    int opcionRival;
    srand(time(NULL));
    for(int i = 0; i < 4; i++){
        partida.codigo[i] = rand() % 10;
        partida.codigotorneo[i] = rand() % 10;

    }
    partida.codigo[4] = '\0';
    partida.codigotorneo[4] = '\0';
    time_t t = time(NULL);
    partida.fecha = *localtime(&t);

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

        if (tipoJuego == 1) {
            partidaDamas(&partida); //partida de damas
        }else if (tipoJuego == 2) {
            menuCuatroRaya(&partida); //partida de cuatro en raya
            printf("ItIsHere");
            csvToDatabasePartida();
        }
        free(listaUsuario);
        return partida;
    case 2:
        if(buscarAmigo()){
            //aqui empezaria la partida con un amigo, (guardar nombre de usuario para los resultados de la partida3)
        
        if (tipoJuego == 1) {
            partidaDamas(&partida); //partida de damas
        } else if (tipoJuego == 2) {
            menuCuatroRaya(&partida); //partida de cuatro en raya
            printf("ItIsHere");
            csvToDatabasePartida();
            
        }
        
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
    //createDB();
    //csvToDatabaseUsuario();
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
void partidaDamas(Partida* partida){
    printf("\nIniciando partida a las Damas...\n");
    Tablero8x8 tableroDamas = crearTableroDamas();
    partida->juego[0] = 'D';
    partida->juego[1] = 'a';
    partida->juego[2] = 'm';
    partida->juego[3] = 'a';
    partida->juego[4] = 's';
    partida->juego[5] = '\0';

    imprimirTableroDamas(tableroDamas);
    int isGameOver = 0;
    int numDamasN = 12;
    int numDamasB = 12;
    int ficha[2];
    //Necesito este string para recoger el input
    char str[4];
    int movimiento;
    FILE * fichero = crearCSVPartida("partidas.csv");
    char nomlog[11];
    sprintf(nomlog,"Logs/LOG%i%i%i.log",partida->fecha.tm_year+1900,partida->fecha.tm_mon,partida->fecha.tm_mday);
    FILE * log = fopen(nomlog, "a");
    fprintf(log, "PartidaLasDamas_%i%i%i%i\n",partida->codigo[0],partida->codigo[1],partida->codigo[2],partida->codigo[3]);
    fflush(log);
    almacenarDatosPartida(partida->codigo,0,partida->juego,partida->fecha,partida->codigotorneo,fichero);
    csvToDatabasePartida();
    //Bucle del juego
    do
    {
        turnoJugador(&tableroDamas, &str[4], movimiento,2, &numDamasB, log);
        printf("Quedan %i fichas blancas\n",numDamasB);
        if (numDamasB <= 0)
        {
            isGameOver = 1;
            fprintf(log,"GANANNEGRAS\n");
            fflush(log);
            partida->resultado = 2;
            break;
        }
        
        turnoJugador(&tableroDamas, &str[4], movimiento,1, &numDamasN, log);
        printf("Quedan %i fichas negras\n",numDamasN);
        if (numDamasN <= 0)
        {
            isGameOver = 1;
            fprintf(log,"GANANBLANCAS\n");
            fflush(log);
            partida->resultado = 1;
        }

        
    } while (isGameOver != 1);
    printf("Ha concluido la partida\n");
    fprintf(log,"FINPARTIDA\n");
    fflush(log);
    fclose(log);
    almacenarDatosPartida(partida->codigo,partida->resultado,partida->juego,partida->fecha,partida->codigotorneo,fichero);
    
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
void turnoJugador(Tablero8x8* tableroDamas, char str[4], int movimiento, int numJugador, int *piezasAdversario, FILE * log){
    int ficha[2];
    int dama;
    int damaReina;
    int sentidoMovimiento;
    if (numJugador == 1)
    {
        dama = 1;
        damaReina = 2;
        sentidoMovimiento = 1;
    }else{
        dama = 3;
        damaReina = 4;
        sentidoMovimiento = -1;   
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
    printf("El jugador %i solo puede coger una de las fichas con captura disponible\n",numJugador);
    fgets(str, 4, stdin);
    ficha[0] = str[0] - 49;
    ficha[1] = str[2] - 49;
    printf("%i\n",ficha[0]);
    printf("%i\n",ficha[1]);
    printf("%i,%i\n", dama, tableroDamas->array[ficha[0]][ficha[1]]);
        }while (!(str[1] == ',' && ficha[0] <9 && ficha[0] >=0 && ficha[1] <9 && ficha[1] >=0 && (tableroDamas->array[ficha[0]][ficha[1]] == dama || tableroDamas->array[ficha[0]][ficha[1]] == damaReina))||fichaEnArray(fichasConPosibleCaptura, contadorPosiblesCapturas,ficha[0],ficha[1]) == 0);
        //Este if sirve para saber si la captura es hacia la izquierda o hacia la derecha.
        if (hayPiezaEnProximidad(*tableroDamas,numJugador,0,ficha[0],ficha[1],0) == 1)
        {
            tableroDamas->array[ficha[0]-sentidoMovimiento*2][ficha[1]-2] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            tableroDamas->array[ficha[0]-sentidoMovimiento][ficha[1]-1] = 0;
            fprintf(log,"%Jugador %i: %i,%ix%i,%i\n",numJugador,ficha[0],ficha[1],ficha[0]-sentidoMovimiento*2,ficha[1]-2);
            fflush(log);
            if (ficha[0] == 7 || ficha[0] == 0)
            {
             tableroDamas->array[ficha[0]-sentidoMovimiento*2][ficha[1]-2] = damaReina;   
            }
            
        }else{
            tableroDamas->array[ficha[0]-sentidoMovimiento*2][ficha[1]+2] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            tableroDamas->array[ficha[0]-sentidoMovimiento][ficha[1]+1] = 0;
            fprintf(log,"%Jugador %i: %i,%ix%i,%i\n",numJugador,ficha[0],ficha[1],ficha[0]-sentidoMovimiento*2,ficha[1]+2);
            fflush(log);
             if (ficha[0]-sentidoMovimiento*2 == 7 || ficha[0]-sentidoMovimiento*2 == 0)
            {
             tableroDamas->array[ficha[0]-sentidoMovimiento*2][ficha[1]+2] = damaReina;
             fprintf(log, "Jugador %i obtiene reina\n",numJugador);   
             fflush(log);
            }
        }
        *piezasAdversario -= 1;
        
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
        } while (!(str[1] == ',' && ficha[0] <9 && ficha[0] >=0 && ficha[1] <9 && ficha[1] >=0 && (tableroDamas->array[ficha[0]][ficha[1]] == dama || tableroDamas->array[ficha[0]][ficha[1]] == damaReina)));
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
            if ((ficha[1] == 0 && movimiento == 1) || (ficha[1] == 7 && movimiento == 2) || (((ficha[0] == 0 && sentidoMovimiento == 1 && isdamaReina ==0) || (ficha[0] == 7 && sentidoMovimiento == -1 && isdamaReina ==0))&& movimiento != 5))
            {
                printf("Este movimiento sacaria la ficha del tablero (invalido)\n");
                goto labelMovimiento;
            }
            
            switch (movimiento)
            {
            case 1:
            if ( tableroDamas->array[ficha[0]-sentidoMovimiento][ficha[1]-1] != 0)
            {
            printf("No puedes ocupar el mismo espacio que otra ficha\n");
            goto labelMovimiento;
            }
                tableroDamas->array[ficha[0]-sentidoMovimiento][ficha[1]-1] = tableroDamas->array[ficha[0]][ficha[1]];
                tableroDamas->array[ficha[0]][ficha[1]] = 0;
                fprintf(log,"%Jugador %i: %i,%i->%i,%i\n",numJugador,ficha[0],ficha[1],ficha[0]-sentidoMovimiento,ficha[1]-1);
                fflush(log);
                break;
            case 2:
            if ( tableroDamas->array[ficha[0]-sentidoMovimiento][ficha[1]+1] != 0)
            {
            printf("No puedes ocupar el mismo espacio que otra ficha\n");
            imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
            goto labelMovimiento;
            }
                tableroDamas->array[ficha[0]-sentidoMovimiento][ficha[1]+1] = tableroDamas->array[ficha[0]][ficha[1]];
                tableroDamas->array[ficha[0]][ficha[1]] = 0;
                fprintf(log,"%Jugador %i: %i,%i->%i,%i\n",numJugador,ficha[0],ficha[1],ficha[0]-sentidoMovimiento,ficha[1]+1);
                fflush(log);
                break;
            case 3:
                if (isdamaReina)
                {
                    if ( tableroDamas->array[ficha[0]+sentidoMovimiento][ficha[1]-1] != 0)
                    {
                    printf("No puedes ocupar el mismo espacio que otra ficha\n");
                    imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
                    goto labelMovimiento;
                    }else{
                    tableroDamas->array[ficha[0]+sentidoMovimiento][ficha[1]-1] = tableroDamas->array[ficha[0]][ficha[1]];
                    tableroDamas->array[ficha[0]][ficha[1]] = 0;
                    fprintf(log,"%Jugador %i: %i,%i->%i,%i\n",numJugador,ficha[0],ficha[1],ficha[0]+sentidoMovimiento,ficha[1]-1);
                    fflush(log);
                }
                }else{
                    goto labelMovimiento;
                }
                break;
            case 4:
            if (isdamaReina)
                {
                    if ( tableroDamas->array[ficha[0]+sentidoMovimiento][ficha[1]+1] != 0)
                    {
                    printf("No puedes ocupar el mismo espacio que otra ficha\n");
                    imprimirTableroDamasconSeleccion(*tableroDamas,ficha[0],ficha[1]);
                    goto labelMovimiento;
                    }else{
                    tableroDamas->array[ficha[0]+sentidoMovimiento][ficha[1]+1] = tableroDamas->array[ficha[0]][ficha[1]];
                    tableroDamas->array[ficha[0]][ficha[1]] = 0;
                    fprintf(log,"%Jugador %i: %i,%i->%i,%i\n",numJugador,ficha[0],ficha[1],ficha[0]+sentidoMovimiento,ficha[1]+1);
                    fflush(log);    
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
                    printf("¡El jugador %i dispone de una nueva reina!\n", numJugador);
                    fprintf(log, "Jugador %i obtiene reina\n",numJugador);
                    fflush(log);
                }
                }
                
            }
            imprimirTableroDamas(*tableroDamas);
            
            }
        
}

//Devuelve 0 si no hay piezas, 1 si la pieza opuesta está a la izquierda, 2 si está a la derecha,
//3 si la pieza es del mismo bando y está a la izquierda, 4 si es del mismo bando y está a la derecha,
//5 si la posición queda fuera del tablero independientemente de la direccion, 6 si queda fuera por la derecha y
//7 si queda fuera por la derecha
int hayPiezaEnProximidad(Tablero8x8 tableroDamas, int bando, int isReina, int filaFicha, int columnaFicha, int preferenciaOutOfBounds){
    int sentidoMovimiento;
    int outOfBoundsLeft = 0;
    int outOfBoundsRight = 0;
    int ajustadorComparacion;
    if (bando == 1)
    {
        sentidoMovimiento = 1;
    }else{
        sentidoMovimiento = -1;
    } if(preferenciaOutOfBounds != 0){
        ajustadorComparacion = 0;
        }else{
            ajustadorComparacion = 1;
        }
    if (!((columnaFicha-1)<(0+ajustadorComparacion) && filaFicha > 7*(bando-1)))
            {
        if ((preferenciaOutOfBounds != 2)&&(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] == 4+(sentidoMovimiento-1)))
        {
                return 1;
            }
        }else{outOfBoundsLeft = 1;
        }
        if (!((columnaFicha+1)>(7-ajustadorComparacion) && filaFicha > 7*(bando-1)))
            {
                if(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] == 4+(sentidoMovimiento-1)){
                return 2;
            }
        }else{outOfBoundsRight = 1;} if(!((columnaFicha-1)<(0+ajustadorComparacion)  && filaFicha > 7*(bando-1)))
            {
                if((preferenciaOutOfBounds != 2)&&(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] != 0)){
                return 3;
            }
        }else{outOfBoundsLeft = 1;}if (!((columnaFicha+1)>(7-ajustadorComparacion) && filaFicha > 7*(bando-1)))
            {
                if(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] != 0){
                return 4;
            }
        }else{outOfBoundsRight = 1;
        }
        if (isReina == 1)
        {
            if (!((columnaFicha-1)<0+ajustadorComparacion))
            {
            if (tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] == 4+(sentidoMovimiento-1))
            {
                return 1;
            }
            }else{outOfBoundsLeft = 1;}if (!((columnaFicha+1)>7-ajustadorComparacion))
            {
                if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] == 4+(sentidoMovimiento-1)){
                return 2;
            }
            }else{outOfBoundsRight = 1;}if (!((columnaFicha-1)<0+ajustadorComparacion))
            {
                if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] != 0){
                return 3;
            }
            
            }else{outOfBoundsLeft = 1;}if (!((columnaFicha+1)>7-ajustadorComparacion))
            {
                if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] != 0){
                return 4;
            }
        }else{outOfBoundsRight = 1;}
        }
        if ((preferenciaOutOfBounds == 1 && outOfBoundsLeft == 1) || (preferenciaOutOfBounds == 2 && outOfBoundsRight == 1) )
        {
            return 5;
        }
        return 0;
        
    /*}else{
        int sentidoMovimiento = -1;
        if (tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] == 1 || tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] == 2)
        {
            return 1;
        }else if(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] == 1 || tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] == 2)
        {
            return 2;
        }
        if (isReina == 1)
        {
            if (tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] == 1 || tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] == 2)
            {
                return 1;
            }else if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] == 1 || tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] == 2){
                return 2;
            }
        }
        return 0;*/
        
}
int hayCapturaDisponible(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha){
    int sentidoMovimiento;
    if (bando == 1)
    {
        sentidoMovimiento = 1;
    }else{
        sentidoMovimiento = -1;
    }
    if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha,0) == 1)
    {
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha-1,1) != 5)
        {
            if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha-1,1) !=3 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha-1,1) !=1 ){
            return 1;
            }
            if (isDama)
            {
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha-1,1) == 5)
            {
                return 0;
            }
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha-1,1) != 3 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha-1,1) != 1){
                return 1;
            }
        }
    
        
        }
        
    }else if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha,0) == 2){
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha+1,2) != 5)
        {
            if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha+1,2) != 4 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha+1,2) != 2){
            return 1;
            }
            if (isDama)
            {
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha+1,2) == 5)
            {
                return 0;
            }
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha+1,2) != 4 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha+1,2) != 2){
                return 1;
                }
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
FILE* crearCSVPartida(char* fichero){
    FILE* fich = fopen(fichero, "w");
    return fich;
}
void almacenarDatosPartida(char codigo[5], int resultado, char juego[15], struct tm fecha,  char codigotorneo[5], FILE * fichero){
    
    fprintf(fichero,"codigoPartida,resultado,juego,anyo,mes,dia,codigoTorneo\n");
    if (fecha.tm_mday <10 && fecha.tm_mon <10)
    {
        fprintf(fichero, "%i%i%i%i,%i,%s,%i,0%i,0%i,%i%i%i%i\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }else if(fecha.tm_mday <10){
        fprintf(fichero, "%i%i%i%i,%i,%s,%i,%i,0%i,%i%i%i%i\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }else if(fecha.tm_mon <10){
        fprintf(fichero, "%i%i%i%i,%i,%s,%i,0%i,%i,%i%i%i%i\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }else{
        fprintf(fichero, "%i%i%i%i,%i,%s,%i,%i,%i,%i%i%i%i\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }
    fclose(fichero);
    //csvToDatabasePartida();
    fichero = NULL;
}

Partida unirsePartida(int tipoJuego){
    //createDB();
    //csvToDatabasePartida();
    //csvToDatabaseUsuario();
    listaPartida = getListaPartida();
    printf("Elija una partida de esta lista introduciendo su codigo: \n\n");
    listaUsuario = getListaUsuario();
    Partida partida;
    int longitud = lineasFichero("partidas.csv");
    char str[10];
    char cod[10];

    printf("Elija una partida de esta lista introduciendo su codigo: \n\n");
    int i;
    for(i = 1; i < longitud-1; i++){
        if (tipoJuego == 1 && strcmp(listaPartida[i].juego,"Damas")==0){
            printf("%d: Codigo: %s, Juego: %s, Fecha: %d-%d-%d\n", i+1, listaPartida[i].codigo, listaPartida[i].juego, listaPartida[i].fecha.tm_year + 1900, listaPartida[i].fecha.tm_mon + 1, listaPartida[i].fecha.tm_mday);
        }else if(tipoJuego == 2 && strcmp(listaPartida[i].juego,"4enRaya")==0){
            printf("%d: Codigo: %s, Juego: %s, Fecha: %d-%d-%d\n", i+1, listaPartida[i].codigo, listaPartida[i].juego, listaPartida[i].fecha.tm_year + 1900, listaPartida[i].fecha.tm_mon + 1, listaPartida[i].fecha.tm_mday);
        }
        /*else if (tipoJuego==2){
            printf("%s\n",listaPartida[i].juego);
        }*/
    }
    printf("Escribe el codigo: ");
    fflush(stdin);
    fgets(cod, 5, stdin);
    fflush(stdin);

    cod[strcspn(cod, "\n")] = '\0';

    int aleatorio = seleccionarJugadorAleatorio();
    for(int i = 0; i < longitud; i++){
        if(strcmp(cod, listaPartida[i].codigo) == 0){
            printf("Contrasenya correcta: \n");
            printf("Uniendo a una partida... \n");
            printf("Jugaras contra <%s> \n", listaUsuario[aleatorio].nombreUsuario);

            //aqui empieza la partida a la que se une
            if (tipoJuego == 1) {
                partida = getPartida(cod);
                partidaDamas(&partida); //partida de damas
                return partida;
            } else if (tipoJuego == 2) {
                partida = getPartida(cod);
                menuCuatroRaya(&partida); //partida de cuatro en raya
                return partida;
            }
            free(listaUsuario);
            return partida;
        }
    }
    
    char opcion[5];
    do {
        printf("El codigo introducido no coincide con ninguna partida.\nQuiere volver a intentarlo? S/N\n");
        fflush(stdin);
        fgets(str, 5, stdin);
        sscanf(str, "%s", &opcion);
    } while (opcion[0] != 's' && opcion[0] != 'S' && opcion[0] != 'n' && opcion[0] != 'N');

    if (opcion[0] == 's' || opcion[0] == 'S') {
        unirsePartida(tipoJuego);
    } else {
        printf("Saliendo del sistema.\n");
        paginaPrincipal();
    }
    

    return partida;
    
}

