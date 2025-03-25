#ifndef REGISTRARSE_H_
#define REGISTRARSE_H_

typedef struct{
    char email[50];
    char nombreUsuario[50];
    char contrasenya[8];
}Usuario;

void menuRegistrarse();

void caseUsuario(char letra);

void menuInicioSesion();

#endif