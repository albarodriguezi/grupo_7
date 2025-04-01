/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "torneo.h"
#include "dibujos.h"
#include "partida.h"
#include "database.h"

Torneo torneos[50];  // Definición real
int numTorneos = 0;  // Definición e inicialización

void cargarTorneosCSV(){
    FILE *file = fopen("torneo.csv", "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir torneos.csv\n");
        return;
    }

    // Saltar cabecera
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);


    numTorneos = 0; // Resetear contador
    while (numTorneos < 50 && 
           fscanf(file, "%d,%49[^,],%49[^,],%49[^,],%49[^\n]",
               &torneos[numTorneos].codt,
               torneos[numTorneos].ganador,
               torneos[numTorneos].fechai,
               torneos[numTorneos].fechaf,
               torneos[numTorneos].nombret) == 5) {
        numTorneos++;
    }
    
    printf("Se cargaron %d torneos\n", numTorneos); // Debug
    if (numTorneos == 0) {
        printf("El archivo está vacío o tiene formato incorrecto\n");
    }
    fclose(file);
}


void metodoEjemploTorneo(){
    printf("estas en la pagina de los torneos");
}

void unirseTorneo() {
    int str;
    printf("**************TORNEOS DISPONIBLES****************\n");
    torneosDisponibles();
    printf("\n");
    printf("*********INTRODUZCA CODIGO TORNEO AL QUE SE QUIERE UNIR********* \n");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (scanf("%d", &str) != 1) {  // Verificar que se leyó correctamente
        printf("Error: Debe ingresar un número válido.\n");
        return;
    }

    for (int i = 0; i < numTorneos; i++) {
        if (torneos[i].codt == str) {
            printf("******************INFO TORNEO**********************");
            printf("\n");
            printf("\tNombre del torneo: %s \n", torneos[i].nombret);
            printf("\tCodigo del torneo: %d \n", torneos[i].codt);
            printf("\tUsuario ganador del torneo: %s \n", torneos[i].ganador);
            printf("\tFecha de incio del torneo: %s \n", torneos[i].fechai);
            printf("\tFecha fin del toreno: %s \n", torneos[i].fechaf);
        
            return;
        }
    }

    printf("codigo no valido");
    torneosDisponibles();
}


void torneosDisponibles() {
    if (numTorneos == 0) {
        printf("No hay torneos disponibles.\n");
        return;
    }

    printf("*********TORNEOS DISPONIBLES*********\n");
    for (int i = 0; i < numTorneos; i++) {
        printf("%d - %s (Código: %d)\n", i + 1, torneos[i].nombret, torneos[i].codt);
    }
}*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "torneo.h"
#include "dibujos.h"
#include "partida.h"
#include "partidaCuatroRaya.h"
#include <time.h>
#include "database.h"


Torneo torneos[50];  // Lista de torneos
int numTorneos = 0;  // Número de torneos cargados

// Cargar torneos de ejemplo en la lista (sin leer de CSV)
void cargarTorneosEjemplo() {
    numTorneos = 8;
    Torneo ejemplos[] = {
        {1001, "blaze41", "2024-03-01", "2025-04-16", "Torneo de Primavera"},
        {1002, "valiant85", "2024-04-10", "2024-04-15", "Final Torneo"},
        {1003, "swift2", "2024-05-20", "2024-05-25", "Abierto principiantes"},
        {1004, "lover_12", "2024-06-12", "2025-06-18", "Torneo de Verano"},
        {1005, "sharp90", "2024-07-05", "2024-07-10", "Torneo de Pros"},
        {1006, "folklore_37", "2024-08-15", "2024-08-20", "Liga de Masters"},
        {1007, "shepard25", "2024-09-03", "2025-09-08", "Torneo de Invierno"},
        {1008, "grim49", "2024-10-22", "2025-10-28", "Torneo de Otonyo"}
    };

    memcpy(torneos, ejemplos, sizeof(ejemplos));
}

time_t convertirFecha(char* fecha){
    struct tm tmFecha = {0};

    // Usamos sscanf para extraer el año, mes y día de la cadena de fecha
    if (sscanf(fecha, "%d-%d-%d", &tmFecha.tm_year, &tmFecha.tm_mon, &tmFecha.tm_mday) == 3) {
        tmFecha.tm_year -= 1900;  // Los años en struct tm empiezan desde 1900
        tmFecha.tm_mon -= 1;      // Los meses en struct tm empiezan desde 0 (enero)

        // Convertir la estructura tm a time_t
        return mktime(&tmFecha);
    } else {
        return -1;  // Devolver -1 si la fecha no tiene el formato correcto
    }
}

void verficarFechaTorneo(Torneo torneo){
    time_t fechaFinTorneo = convertirFecha(torneo.fechaf);
    time_t ahora;
    time(&ahora);

    if(ahora < fechaFinTorneo){
        printf("el torneo no ha finalizado");
    }else{
        printf("el torneo ya ha finalizado");
    }
}

void unirseTorneo() {
    cargarTorneosEjemplo();
    int codigoTorneo;
    printf("*************************TORNEOS DISPONIBLES****************************\n");
    torneosDisponibles();
    printf("\n*******************INTRODUZCA CODIGO TORNEO AL QUE SE QUIERE UNIR******************\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (scanf("%d", &codigoTorneo) != 1) {
        printf("Error: Debe ingresar un número válido.\n");
        return;
    }

    for (int i = 0; i < numTorneos; i++) {
        if (torneos[i].codt == codigoTorneo) {
            printf("*******************************INFO TORNEO***********************************\n");
            printf("\tNombre del torneo: %s \n", torneos[i].nombret);
            printf("\tCodigo del torneo: %d \n", torneos[i].codt);
            printf("\tUsuario ganador del torneo: %s \n", torneos[i].ganador);
            printf("\tFecha de inicio del torneo: %s \n", torneos[i].fechai);
            printf("\tFecha fin del torneo: %s \n", torneos[i].fechaf);

            verficarFechaTorneo(torneos[i]);

            /*
            Partida p;
            Partida* listaPartida = getListaPartida();
            for(int i = 0; i < lineasFichero("partidas.csv");i++){
                p = listaPartida[i];
            }*/
            
            Partida partidaEjemplo = {
                "P001",               // Código de la partida
                1,                    // Resultado: 1 significa que el jugador 1 ganó
                true,                 // Activa: la partida está activa
                "CuatroRaya",         // Nombre del juego
                {2024, 3, 10, 12, 30, 0},  // Fecha: 10 de marzo de 2024, a las 12:30
                "Jugador 1: Coloca ficha en columna 3, Jugador 2: Coloca ficha en columna 4", // Registro de movimientos
                false,                // No es amistosa, es competitiva
                "T001"   }; 
            
            
            menuCuatroRaya(&partidaEjemplo);
            return ;

            
        }



    }

    printf("Código no válido\n");
    torneosDisponibles();
}

void torneosDisponibles() {
    cargarTorneosEjemplo();
    if (numTorneos == 0) {
        printf("No hay torneos disponibles.\n");
        return;
    }

    printf("***********************TORNEOS DISPONIBLES******************************\n");
    for (int i = 0; i < numTorneos; i++) {
        printf("\t %d. %s (Codigo: %d)\n", i+1, torneos[i].nombret, torneos[i].codt);
    }
}
