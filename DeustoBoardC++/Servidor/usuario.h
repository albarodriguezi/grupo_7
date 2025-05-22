#ifndef USUARIO_H_
#define USUARIO_H_

class Usuario {
private:
    char email[50];
    char nombreUsuario[50];
    char contrasenya[50];

public:
	Usuario(char* email, char* nombreUsuario, char* constrasenya);
	Usuario();
	virtual ~Usuario();
	Usuario(const Usuario &other);
	char* getEmail();
	void setEmail(char* email);
	char* getNombre();
	void setNombre(char* nombre);
	char* getConstrasenya();
	void setContrasenya(char* contrasenya);
	
};

#endif