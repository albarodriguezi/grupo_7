#include "usuario.h"
#include <iostream>
#include <cstring>

Usuario::Usuario(char* email, char* nombre, char* contrasenya){
	strcpy(this->email, email);
	strcpy(this->nombreUsuario, nombre);
	strcpy(this->contrasenya, contrasenya);
}

Usuario::Usuario(){
	strcpy(this->email, "");
	strcpy(this->nombreUsuario, "");
	strcpy(this->contrasenya, "");
	
}

Usuario::~Usuario() {
}

Usuario::Usuario(const Usuario &other) {
	strcpy(this->email, other.email);
	strcpy(this->nombreUsuario, other.nombreUsuario);
	strcpy(this->contrasenya, other.contrasenya);
}

char* Usuario::getEmail() {
	return this->email;
}

void Usuario::setEmail(char *email) {
	strcpy(this->email, email);
}

char* Usuario::getNombre() {
	return this->nombreUsuario;
}

void Usuario::setNombre(char *nombre) {
	strcpy(this->nombreUsuario, nombre);
}

char* Usuario::getConstrasenya() {
	return this->contrasenya;
}

void Usuario::setContrasenya(char *apellido) {
	strcpy(this->contrasenya, apellido);
}
