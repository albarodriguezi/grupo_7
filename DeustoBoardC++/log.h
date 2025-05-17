#ifndef LOG_H_
#define LOG_H_
#include <iostream>
#include <fstream>
#include <ctime>
#include "stdlib.h"
#include "stdio.h"
#include <cstring>
class Log {

private:
	char* nombreArchivo;
	FILE* archivo;
public:
	Log(const char* nombreArchivo);
	virtual ~Log();

	void anadirLog(const char* mensaje);
};

#endif 