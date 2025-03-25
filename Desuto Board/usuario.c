#include <stdio.h>
#include "usuario.h"
#include "dibujos.h"

void menuRegistrarse(){

    Usuario u;

    char tecla; 

    dibujoPerfil();

    printf("*********************************************************************");
    printf("************************REGISTRO USUARIO*****************************");
    printf("*********************************************************************");

    printf("Introduce el email: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.email);

	printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.nombreUsuario);

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.contrasenya);

    printf("\n");
    printf("Cuenta creada. Para continuar presione la tecla I");
    fflush(stdout);
	fflush(stdin);
	scanf("%d",&tecla);

    caseUsuario(tecla);


}

void caseUsuario(int tecla){

    system("cls");
	switch (tecla) {
        case 'I':
            menuInicioSesion();
            break;
        case 'i':
            menuInicioSesion();

    }
}

void menuInicioSesion(){

    Usuario u;
    char tecla;

    dibujoPerfil();

    printf("*********************************************************************");
    printf("************************INICIO DE SESION*****************************");
    printf("*********************************************************************");

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.nombreUsuario);

    printf("Introduce el nombre de usuario: ");
	fflush(stdout);
	fflush(stdin);
	gets(u.contrasenya);

    printf("\n");
    printf("Inicio de sesion. Para continuar presione la tecla I");
    fflush(stdout);
	fflush(stdin);
	scanf("%d",&tecla);

    caseUsuario(tecla);

}