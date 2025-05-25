#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "dibujos.h"
#include "principal.h"
#include "database.h"


void menuRegistrarse();
void caseRegistro(char tecla, Usuario u);
void caseInicioSesion(char tecla, Usuario u);
void menuInicioSesion();


void menuRegistrarse(){

    Usuario u;

    char bufer[50];

    char tecla; 

    dibujoPerfil();
    printf("\n");

    printf("********************************************************************* \n");
    printf("************************REGISTRO USUARIO***************************** \n");
    printf("********************************************************************* \n");

    printf("Introduce el email: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.email);


	printf("Introduce el nombre de usuario: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.nombreUsuario);

    printf("Introduce la contrasenya de usuario: ");
	fflush(stdout);
    fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%s", u.contrasenya);

    printf("\n");
    printf("Registro de usuario. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%c", &tecla);

    caseRegistro(tecla, u);


}



void menuInicioSesion(){

    Usuario u;

    char bufer[50];

    char tecla;

    dibujoPerfil();
    printf("\n");

    printf("********************************************************************* \n");
    printf("************************INICIO DE SESION***************************** \n");
    printf("********************************************************************* \n");
    int control = 0;
    while(control == 0){
        printf("Introduce el email del usuario: ");
	    fflush(stdout);
        fflush(stdin);
        char email[50];
        char contrasenya[50];
        fgets(bufer, sizeof(bufer), stdin);
        bufer[strcspn(bufer, "\n")] = 0;
        sscanf(bufer, "%s", email);

        printf("Introduce la contrasenya de usuario: ");
	    fflush(stdout);
	    fflush(stdin);
        fgets(bufer, sizeof(bufer), stdin);
        bufer[strcspn(bufer, "\n")] = 0;
        sscanf(bufer, "%s", contrasenya);
        Usuario un = getUsuario(email);
        if (strcmp(un.contrasenya,contrasenya)==0){
            control = 1;
            u = un;
        }else{
            printf("Contrasenya incorrecta.\n");
            printf("%i\n",contrasenya);
            printf("%i\n",un.contrasenya);
        }
    }
  

    printf("\n");
    printf("Inicio de sesion. \n");
    printf("Para continuar presione la tecla I \n");
    printf("Para salir pulse la tecla E \n");
    fflush(stdout);
	fflush(stdin);

    fgets(bufer, sizeof(bufer), stdin);
    sscanf(bufer, "%c", &tecla);

    caseInicioSesion(tecla, u);

}

void caseRegistro(char tecla, Usuario u){

    system("cls");
	switch (tecla) {
        case 'I':
            saveUsuario(u);
            menuInicioSesion();
            printf("registro correcto");
            break;
        case 'i':
            saveUsuario(u);
            menuInicioSesion();
            printf("registro correcto");
            break;
        case 'E':
            printf("saliendo de DEUSTO BOARD");
            break;
        case 'e':
            printf("saliendo de DEUSTO BOARD");
            break;
        default:
            printf("Tecla inválida. Inténtalo de nuevo.\n");
            menuRegistrarse(tecla);
            break;
    }
}

void caseInicioSesion(char tecla, Usuario u){

    system("cls");
	switch (tecla) {
        case 'I':
            //obtenemos el usuario
            {Usuario uDB = getUsuario(u.email);  
            
        // comprobamos que el email existe en la base de datos
        if (strlen(uDB.email) == 0) {
            printf("No existe una cuenta con ese email.\n");
            menuInicioSesion();
            return;
        }
        
        // comprobamos que la contraseña coincida con la contraseña del usuario en el dataset
        if (uDB.contrasenya !=  u.contrasenya) {
            printf("Contrasenya correcta.\n");
            //elegirModoJuego();
            return;
        }else{
            printf("Contrasenya incorrecta\n");
            menuInicioSesion();
        }
        
        // 
        printf("Inicio de sesión correcto\n");
        //elegirModoJuego();
        }
        break;
        case 'i':
            {Usuario uDB = getUsuario(u.email);
            
                //comprobamos si el usuario existe
                if (strlen(uDB.email) == 0) {
                    printf("Error: No existe una cuenta con ese email.\n");
                    menuInicioSesion();
                    return;
                }
                
                // Verificar contraseña
                if (uDB.contrasenya !=  u.contrasenya) {
                    printf("Contrasenya correcta.\n");
                    //elegirModoJuego();
                    return;
                }else{
                    printf("Contrasenya incorrecta\n");
                    menuInicioSesion();
                }
                
            }
            break;
        case 'E':
            printf("saliendo de DEUSTO BOARD");
            break;
        case 'e':
            printf("saliendo de DEUSTO BOARD");
            break;
        default:
            printf("Tecla invalida. Intentalo de nuevo.\n");
            menuInicioSesion(tecla);
            break;
    }
}