


#ifndef TORNEO_H_
#define TORNEO_H_

typedef struct{
    int codt;
    char ganador[50];
    char fechai[50];
    char fechaf[50];
    char nombret[50];
}Torneo;



//void metodoEjemploTorneo();
//void cargarTorneosCSV();
void cargarTorneosEjemplo();
void unirseTorneo();
void torneosDisponibles();

#endif