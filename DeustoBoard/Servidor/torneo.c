#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "torneo.h"
#include "dibujos.h"
#include "servidorPruebaPartida.h"
#include <time.h>
#include "database.h"

Torneo *torneos;    // Lista de torneos
int numTorneos = 0; // Número de torneos cargados

void cargarTorneosEjemplo()
{
    torneos = getListaTorneo();
}

time_t convertirFecha(char *fecha)
{
    struct tm tmFecha = {0};

    // Usamos sscanf para extraer el año, mes y día de la cadena de fecha
    if (sscanf(fecha, "%d-%d-%d", &tmFecha.tm_year, &tmFecha.tm_mon, &tmFecha.tm_mday) == 3)
    {
        tmFecha.tm_year -= 1900; // Los años en struct tm empiezan desde 1900
        tmFecha.tm_mon -= 1;     // Los meses en struct tm empiezan desde 0 (enero)

        // Convertir la estructura tm a time_t
        return mktime(&tmFecha);
    }
    else
    {
        return -1; // Devolver -1 si la fecha no tiene el formato correcto
    }
}

void verficarFechaTorneo(Torneo torneo)
{
    time_t fechaFinTorneo = convertirFecha(torneo.fechaf);
    time_t ahora;
    time(&ahora);

    if (ahora < fechaFinTorneo)
    {
        printf("el torneo no ha finalizado");
    }
    else
    {
        printf("el torneo ya ha finalizado");
    }
}

void unirseTorneo(int modoJuegoSeleccionado)
{
    cargarTorneosEjemplo();
    int codigoTorneo;
    printf("*************************TORNEOS DISPONIBLES****************************\n");
    numTorneos = lineasFichero("torneo.csv");
    cargarTorneosEjemplo();
    if (numTorneos == 0)
    {
        printf("No hay torneos disponibles.\n");
        return;
    }

    for (int i = 0; i < numTorneos - 1; i++)
    {
        printf("\t %d. %s (Codigo: %s)\n", i + 1, torneos[i].nombret, torneos[i].codt);
    }
    printf("\n*******************INTRODUZCA CODIGO TORNEO AL QUE SE QUIERE UNIR******************\n");
    numTorneos = lineasFichero("torneo.csv");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    if (scanf("%d", &codigoTorneo) != 1)
    {
        printf("Error: Debe ingresar un numero valido.\n");
        return;
    }

    char codtchar[5];
    snprintf(codtchar, sizeof(codtchar), "%d", codigoTorneo);

    for (int i = 0; i < numTorneos; i++)
    {
        if (strcmp(torneos[i].codt, codtchar) == 0)
        {
            printf("*******************************INFO TORNEO***********************************\n");
            printf("\tNombre del torneo: %s \n", torneos[i].nombret);
            printf("\tCodigo del torneo: %s \n", torneos[i].codt);
            printf("\tUsuario ganador del torneo: %s \n", torneos[i].ganador);
            printf("\tFecha de inicio del torneo: %s \n", torneos[i].fechai);
            printf("\tFecha fin del torneo: %s \n", torneos[i].fechaf);

            Partida partidaEjemplo;
            strcpy(partidaEjemplo.codigo, "9999");
            partidaEjemplo.amistosoCom = 1;
            strcpy(partidaEjemplo.codigotorneo, codtchar);
            // Obtener el tiempo actual
            time_t t = time(NULL);

            // Convertirlo a la estructura tm (hora local)
            struct tm tmFecha = *localtime(&t);
            partidaEjemplo.fecha = tmFecha;
            if (modoJuegoSeleccionado == 1)
            {
                strcpy(partidaEjemplo.juego, "Damas");
            }
            else if (modoJuegoSeleccionado == 2)
            {
                strcpy(partidaEjemplo.juego, "CuatroEnRaya");
            }

            return;
        }
    }

    printf("Código no válido\n");
}

void torneosDisponibles(int modoJuegoSeleccionado)
{
    numTorneos = lineasFichero("torneo.csv");
    cargarTorneosEjemplo();
    if (numTorneos == 0)
    {
        printf("No hay torneos disponibles.\n");
        return;
    }
    for (int i = 0; i < numTorneos - 1; i++)
    {
        printf("\t %d. %s (Codigo: %s)\n", i + 1, torneos[i].nombret, torneos[i].codt);
    }

    fflush(stdin);
    char str[5];
    printf("\nClick enter to exit: \n");
    fgets(str, sizeof(str), stdin);
    fflush(stdin);
}