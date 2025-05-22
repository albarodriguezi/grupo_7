
#ifndef FICHERO_H_
#define FICHERO_H_

class Fichero {
private:
	char database[100];
	char log[100];
	char ip[20];
	int puerto;
public:
	Fichero();
	virtual ~Fichero();
	void leerConfig();

	char* getDatabase();
	void setDatabase(char* database);

	char* getLog();
	void setLog(char* log);

	char* getIp();
	void setIp(char* ip);

	int getPuerto() const;
	void setPuerto(int puerto);
};

#endif