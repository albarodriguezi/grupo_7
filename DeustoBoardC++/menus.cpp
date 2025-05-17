#include "menus.h"
#include "log.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <winsock2.h>
#include <windows.h>

//menu principal(registro, inicio de sesion, salir de la pagina)
void menuPrincipal(SOCKET* s, Log& logger) {
	system("cls");
	int opcion;
	dibujoDeustoBoard();
	cout<<"1. Registrarse"<<endl<<
		  "2. Iniciar sesion"<<endl<<
		  "0. Salir"<<endl<<endl<<
		  "Introduce una opcion: "; 
    cin>>opcion;
    cout<<endl;
    caseMenuPrincipal(&opcion, s, logger);

}

//case para el menu principal
void caseMenuPrincipal(int *opcion, SOCKET* s, Log& logger) {
	system("cls");
	switch (*opcion) {
			case 1:
                menuRegistrarse(s, logger);
				break;
			case 2:
				menuInicioSesion(s, logger);
				break;
			case 0:
				enviarComandoSalir(s);
				logger.anadirLog("Programa cerrado correctamente");
				cout<<"SALIENDO...";
				exit(0);
			default:
				cout<<"El digito introducido no corresponde a ninguno de los anteriores"<<endl;
				menuPrincipal(s, logger);
				break;
		}
}

//menu para registrarse ( pide email, nombre usuairo y contraseña)
void menuRegistrarse(SOCKET* s, Log& logger) {
	dibujoPerfil();

    cout <<"*********************************************************************" << endl <<
           "************************REGISTRO USUARIO*****************************"<< endl <<
           "*********************************************************************" <<endl<<endl;

	char email[50];
    char nombreUsuario[50];
	char contrasenya[50];
	cout<<"Inserte nombre usuario: ";
	cin>>nombreUsuario;
    cout<<"Inserte email: ";
	cin>>email;
	cout<<"Inserte contrasena: ";
	cin>>contrasenya;

	Usuario u;
    u.setNombre(nombreUsuario);
	u.setEmail(email);
	u.setContrasenya(contrasenya);
	enviarComandoRegistro(s, u);


    int existe = 1;

	if (existe == 1) {
		cout << "Accediendo...." << endl;

		logger.anadirLog("Inicio de sesion correcto");

		Sleep(3000);
        //hay que crear el menu de la pagina principal
		//paginaPrincipal(s, u, logger);
        menuInicioSesion(s, logger);
	}

	cout << "Error al iniciar sesion." << endl;

	logger.anadirLog("Error al iniciar sesion");

	Sleep(3000);
	menuPrincipal(s, logger);


}

//menu inicio de sesion (pide email y constraseña)
void menuInicioSesion(SOCKET* s, Log& logger) {
	dibujoPerfil();

    cout <<"*********************************************************************" << endl <<
           "************************INICIO DE SESION*****************************"<< endl <<
           "*********************************************************************" <<endl<<endl;

	char email[50];
	char contrasenya[50];
	cout<<"Inserte dni: ";
	cin>>email;
	cout<<"Inserte contrasena: ";
	cin>>contrasenya;

	Usuario u;
	int existe = enviarComandoIniciarSesion(s, email, contrasenya, u);



	if (existe == 1) {

		cout << "Accediendo..." << endl;


		logger.anadirLog("Inicio de sesion correcto");

		Sleep(3000);
        //crear menu de partida principal
		//paginaPrincipal(s, u, logger);
	}


	cout << "Error al iniciar sesion." << endl;


	logger.anadirLog("Error al iniciar sesion");

	Sleep(3000);
	menuPrincipal(s, logger);


}

//menu pagina principal de juego( crear partida, unirse partida, unirse torneo...)
void menuPaginaPrincipal(SOCKET* s, Log& logger) {

	cout <<"*********************************************************************" << endl <<
	"************************ELIGA FORMA DE JUEGO*****************************"<< endl <<
	"*********************************************************************" <<endl<<endl;
	system("cls");
	int opcion;
	dibujoPaginaPrincipal();
	cout<<"1. Crear partida "<<endl<<
		  "2.  Unirse partida"<<endl<<
		  "3.  Unirse Torneo"<<endl<<
		  "4.  Ver partidas disponibles"<<endl<<
		  "5.  Ver torneos disponibles"<<endl<<
		  "6. Exit" <<endl<<endl<<
		  "Introduce una opcion: "; 
    cin>>opcion;
    cout<<endl;
    casePaginaPrincipal(&opcion, s, logger);

}

//case pagina principal
void casePaginaPrincipal(int *opcion, SOCKET* s, Log& logger) {
	system("cls");
	switch (*opcion) {
			case 1:
                //hay que añadir estos metodos
				//ELEMPLO --> menuRegistrarse(s, logger);
				break;
			case 2:
				//hay que añadir estos metodos
				break;
			case 3:
				//hay que añadir estos metodos
				break;
			case 4:
				//hay que añadir estos metodos
				break;
			case 5:
				//hay que añadir estos metodos
				break;
			case 6:
				enviarComandoSalir(s);
				logger.anadirLog("Programa cerrado correctamente");
				cout<<"SALIENDO...";
				exit(0);
			default:
				cout<<"El digito introducido no corresponde a ninguno de los anteriores"<<endl;
				menuPaginaPrincipal(s, logger);
				break;
		}
}

int modoJuegoSeleccionado = 1;

//menu para elegir el juego al que se quiere jugar(damas o cuatro en raya)
void menuModoJuego(SOCKET* s, Log& logger) {

	cout << "*********************************************************************" << endl <<
			"************************ELIGA JUEGO**********************************"<< endl <<
			"*********************************************************************" <<endl<<endl;
	system("cls");
	int opcion;
	dibujoPaginaPrincipal();
	cout<<"1. Damas"<<endl<<
		  "2. Cuatro en raya"<<endl<<
		  "3. Exit" <<endl<<endl<<
		  "Introduce una opcion: "; 
    cin>>opcion;
    cout<<endl;
    casePaginaPrincipal(&opcion, s, logger);

}

//case del menu do modo de juego
void caseModoJuego(int *opcion, SOCKET* s, Log& logger) {
	system("cls");
	switch (*opcion) {
			case 1:
                menuPaginaPrincipal(s, logger);
				modoJuegoSeleccionado = 1;
				break;
			case 2:
			    menuPaginaPrincipal(s, logger);
				modoJuegoSeleccionado = 2;
				break;
			case 6:
				enviarComandoSalir(s);
				logger.anadirLog("Programa cerrado correctamente");
				cout<<"SALIENDO...";
				exit(0);
			default:
				cout<<"El digito introducido no corresponde a ninguno de los anteriores"<<endl;
				menuModoJuego(s, logger);
				break;
		}
}

//--------------------------------------------SOCKET---------------------------------------------------------------


//comando salir
void enviarComandoSalir(SOCKET *s) {
	char sendBuff[512];
	strcpy(sendBuff, "SALIR");
	send(*s, sendBuff, sizeof(sendBuff), 0);
}

//comando iniciar sesion
int enviarComandoIniciarSesion(SOCKET* s, char* email,char* contrasena, Usuario& u){
	char sendBuff[512], recvBuff[512];

	strcpy(sendBuff, "COMP_INICIO_SESION");
	send(*s, sendBuff, sizeof(sendBuff), 0);
	strcpy(sendBuff, email);
    send(*s, sendBuff, sizeof(sendBuff), 0);
	strcpy(sendBuff, contrasena);
	send(*s, sendBuff, sizeof(sendBuff), 0);


	recv(*s, recvBuff, sizeof(recvBuff), 0);
	u.setEmail(recvBuff);
	recv(*s, recvBuff, sizeof(recvBuff), 0);
	u.setContrasenya(recvBuff);

	recv(*s, recvBuff, sizeof(recvBuff), 0);


	return atoi(recvBuff);

}

//comando registro 
void enviarComandoRegistro(SOCKET* s, Usuario& u) {
	char sendBuff[512];

	strcpy(sendBuff, "COMP_REGISTRO");
	send(*s, sendBuff, sizeof(sendBuff), 0);
	strcpy(sendBuff, u.getEmail());
	send(*s, sendBuff, sizeof(sendBuff), 0);
	strcpy(sendBuff, u.getNombre());
	send(*s, sendBuff, sizeof(sendBuff), 0);
	strcpy(sendBuff, u.getContrasenya());
}