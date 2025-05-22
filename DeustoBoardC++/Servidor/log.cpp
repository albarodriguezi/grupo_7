#include "log.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
using namespace std;

Log::Log(const char* nombreArchivo) {
	this->nombreArchivo = new char[strlen(nombreArchivo)+1];
	strcpy(this->nombreArchivo, nombreArchivo);
	this->archivo = NULL;
}

Log::~Log() {
	if (archivo) {
		fclose(archivo);
	}
	delete[] this->nombreArchivo;
}


void Log::anadirLog(const char *mensaje) {
	this->archivo = fopen(this->nombreArchivo, "a");
	if (!this->archivo) { // si no existe?!
	   cout << "No se puede abrir el archivo de logger: " << nombreArchivo << endl;
	}

	if (archivo) {
		// sacar hora actual
		time_t ahora = time(nullptr);

		// guardar hora actual
		char* tiempo = ctime(&ahora);

		// Eliminar el caracter de nueva línea
		tiempo[strlen(tiempo) - 1] = '\0';

		// Aplicar formato [hora] mensaje
		fprintf(this->archivo, "[%s] %s\n", tiempo, mensaje);
		fclose(this->archivo);
	}
}