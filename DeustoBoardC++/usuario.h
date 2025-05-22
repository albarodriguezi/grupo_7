#include <winsock2.h>
#include "socket.h"
#ifndef USUARIO_H_
#define USUARIO_H_

class Usuario{
    private:
        char email[50];
        char nombreUsuario[50];
        char contrasenya[50];
    public:
        Usuario(char* email, char* nombre, char* contra);
        Usuario();
        virtual ~Usuario();
        Usuario(const Usuario &other);
        
        //getters
        char* getEmail();
        char* getNombre();
        char* getContrasenya();

        //setters
        void setEmail(char* email);
        void setNombre(char* nombreUsuario);
        void setContrasenya(char* contrasenya);

        void pedirUsuario(SOCKET* s);
};

#endif