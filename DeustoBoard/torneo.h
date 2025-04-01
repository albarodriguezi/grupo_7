


#ifndef TORNEO_H_
#define TORNEO_H_
#include <time.h>

typedef struct{
    char codt[5];
    char ganador[50];
    char fechai[50];
    char fechaf[50];
    char nombret[50];
}Torneo;



//void metodoEjemploTorneo();
//void cargarTorneosCSV();
time_t convertirFecha(char* fecha);
void cargarTorneosEjemplo();
void unirseTorneo(int modoJuegoSeleccionado);
void torneosDisponibles(int opcion);

#endif