#include "usuario.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "socket.h"
using namespace std;

Usuario :: Usuario(char* email, char* nombre, char* contra){
    strcpy(this->email, email);
    strcpy(this->nombreUsuario, nombre);
    strcpy(this->contrasenya, contra);
}

Usuario :: Usuario(){
    strcpy(this->email, "");
    strcpy(this->nombreUsuario, "");
    strcpy(this->contrasenya, "");
}

Usuario :: ~Usuario(){}

Usuario :: Usuario(const Usuario &otro){
    strcpy(this->email, otro.email);
    strcpy(this->nombreUsuario, otro.nombreUsuario);
    strcpy(this->contrasenya, otro.contrasenya);
}

char* Usuario ::getEmail(){
    return this->email;
}
char* Usuario :: getNombre(){
    return this->nombreUsuario;
}
char* Usuario :: getContrasenya(){
    return this->contrasenya;
}

void Usuario :: setEmail(char* email){
    strcpy(this->email, email);
}
void Usuario :: setNombre(char* nombreUsuario){
    strcpy(this->nombreUsuario, nombreUsuario);
}
void Usuario :: setContrasenya(char* contrasenya){
    strcpy(this->contrasenya, contrasenya);
}

void Usuario::pedirUsuario(SOCKET* s) {
    char email[50];
    char nombreUsuario[50];
    char contrasenya[50];
    char sendBuff[512];
    char recvBuff[1024];

	cout << "Introduce el DNI: ";
    cin >> email;
    cout << endl;
	setEmail(email);
	cout << "Introduce el nombre: ";
    cin >> nombreUsuario;
    cout << endl;
	setNombre(nombreUsuario);
	cout << "Introduce el apellido: ";
    cin >> contrasenya;
    cout << endl;
	setContrasenya(contrasenya);

    //char* mensaje=new char[strlen(email)+strlen(nombreUsuario)+strlen(contrasenya)+8];
    
}

