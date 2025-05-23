#include "menus.h"
#include "log.h"
#include "socket.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <winsock2.h>
#include <windows.h>

//#include "usuario.h"

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
        //te lleva al menu de inicio de sesion
        menuInicioSesion(s, logger);
	}

	cout << "Error al iniciar sesion." << endl;

	logger.anadirLog("Error al iniciar sesion");

	Sleep(3000);
	menuPrincipal(s, logger);


}

//menu inicio de sesion (pide email y constraseña)
/*
void menuInicioSesion(SOCKET* s, Log& logger) {
	dibujoPerfil();

    cout <<"*********************************************************************" << endl <<
           "************************INICIO DE SESION*****************************"<< endl <<
           "*********************************************************************" <<endl<<endl;

	char email[50];
	char contrasenya[50];
	cout<<"Inserte email: ";
	cin>>email;
	cout<<"Inserte contrasena: ";
	cin>>contrasenya;

	int existe = enviarComandoIniciarSesion(s, email, contrasenya);



	if (existe == 1) {

		cout << "Accediendo..." << endl;


		logger.anadirLog("Inicio de sesion correcto");

		Sleep(3000);
		//te lleva al menu para elegir el juego qeu quieres jugar
		menuModoJuego(s, logger);
	}


	cout << "Error al iniciar sesion." << endl;
	logger.anadirLog("Error al iniciar sesion");

	Sleep(3000);
	//hay que cambiarlo --> deveria llevarte a la pagina de inicio otra vez
	menuInicioSesion(s, logger);


}
*/

void menuInicioSesion(SOCKET* s, Log& logger) {
	while (true) {
		dibujoPerfil();

		cout << "************************INICIO DE SESION*****************************" << endl;
		char email[50];
		char contrasenya[50];

		cout << "Inserte email: ";
		cin >> email;
		cout << "Inserte contrasena: ";
		cin >> contrasenya;

		int existe = enviarComandoIniciarSesion(s, email, contrasenya);

		if (existe == 1) {
			cout << "Accediendo..." << endl;
			logger.anadirLog("Inicio de sesion correcto");
			Sleep(3000);
			menuModoJuego(s, logger);
			return;
		} else {
			cout << "Error al iniciar sesion.\n" << endl;
			logger.anadirLog("Error al iniciar sesion");
			Sleep(3000);
		}
	}
}


//menu pagina principal de juego( crear partida, unirse partida, unirse torneo...)
void menuPaginaPrincipal(SOCKET* s, Log& logger) {
	system("cls");
	int opcion;
	dibujoPaginaPrincipal();
		cout <<"*********************************************************************" << endl <<
	"************************ELIGA FORMA DE JUEGO*****************************"<< endl <<
	"*********************************************************************" <<endl<<endl;
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
				crearPartida(opcion, s, logger);
				menuPaginaPrincipal(s,logger);
				//ELEMPLO --> menuRegistrarse(s, logger);
				break;
			case 2:
				menuUnirsePartida(s,logger);
				menuPaginaPrincipal(s,logger);
				break;
			case 3:
				menuUnirseTorneo(s,logger);
				menuPaginaPrincipal(s,logger);
				break;
			case 4:
				
				menuPartidasDisponibles(s,logger);
				menuPaginaPrincipal(s,logger);
				break;
			case 5:
				menuTorneosDisponibles(s, logger);
				menuPaginaPrincipal(s,logger);
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
	system("cls");
	int opcion;
		cout << "*********************************************************************" << endl <<
			"************************ELIGA JUEGO**********************************"<< endl <<
			"*********************************************************************" <<endl<<endl;
	cout<<"1. Damas"<<endl<<
		  "2. Cuatro en raya"<<endl<<
		  "3. Exit" <<endl<<endl<<
		  "Introduce una opcion: "; 
    cin>>opcion;
    cout<<endl;
    caseModoJuego(&opcion, s, logger);

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
/*
void menuPartidasDisponibles(SOCKET *s, Log &logger)
{
	system("cls");
	cout << "*********************************************************************" << endl <<
			"************************PARTIDAS DISPONIBLES*************************"<< endl <<
			"*********************************************************************" <<endl<<endl;
	if (modoJuegoSeleccionado==1){
		char sendBuff[512];
		char recvBuff[1024];
		recvBuff[0]='\0';
		strcpy(sendBuff, "GPA::Damas");
    	send(*s, sendBuff, sizeof(sendBuff), 0);
		//sleep(100);
		recv(*s, recvBuff, sizeof(recvBuff), 0);
		int tamano = contarDobleSlash(recvBuff);
		cout << recvBuff;
		char* token = strtok(recvBuff, "//");
		while (token != nullptr) {
		int i=0;
		char* tokendos = strtok(token, ";");
		int j=0;
		char codigo[5];
		char juego[6];
		char fecha[11];
        while (tokendos != nullptr) {
			if (j==0){
				strcpy(codigo,tokendos);
			}else if(j==1){
				strcpy(juego,tokendos);
			}else if(j==2){
				strcpy(fecha,tokendos);
			}
			j++;
        }
		cout << i+1 << ". " << juego <<" (" << fecha << ")"<< endl;
        token = strtok(nullptr, "//");  // avanzar al próximo token
		i++;
    }
	cout << "Type something to get back";
	char* enter;
	cin >> enter;
	}else if(modoJuegoSeleccionado==2){

	}else{
		cout << modoJuegoSeleccionado;
	}
}

void menuPartidasDisponibles(SOCKET *s, Log &logger)
{
	system("cls");
	cout << "*********************************************************************" << endl
		 << "************************PARTIDAS DISPONIBLES*************************" << endl
		 << "*********************************************************************" << endl
		 << endl;
	cout << "Give us a moment..." << endl;

	if (modoJuegoSeleccionado == 1) {
		char sendBuff[512];
		char recvBuff[1024];
		char juego[6];
		char fecha[11];
		char codigo[5];
		recvBuff[0] = '\0';


		strcpy(sendBuff, "GPA::Damas");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		recv(*s, recvBuff, sizeof(recvBuff), 0);
		int tamano = contarSlash(recvBuff);
		//cout << tamano << endl;
		int j=0;
		while (j<tamano){
		char* listaPartidas[tamano];
		char *token = strtok(recvBuff, "/");
		int j = 0;
		while (j < tamano) {
			listaPartidas[j] = token;
			token = strtok(NULL, "/");
		}
		j=0;
		
		while (j< tamano){
				char *tokendos = strtok(token, ";");
				int i=0;
				while (i<3){
        			if (i == 0) {
            			strcpy(codigo, tokendos);
        			} else if (i == 1) {
            			strcpy(juego, tokendos);
        			} else if (i == 2) {
						strcpy(fecha, tokendos);
						cout << j << ". " << codigo << endl;
        			}
        			i++;
        			tokendos = strtok(NULL, ";");
				}
    		}
			j++;
	}
		cout << "\nPulsa ENTER para volver...";
		cin.ignore();
		cin.get();
	
	}else if (modoJuegoSeleccionado == 2) {
		// Otro modo
	}
	else {
		cout << modoJuegoSeleccionado;
	}
}
*/
//----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void crearPartida(int *opcion, SOCKET* s, Log& logger) {
    system("cls");
    cout << "*********************************************************************" << endl
         << "*************************** CREAR PARTIDA ***************************" << endl
         << "*********************************************************************" << endl
         << endl;

    cout << "1. Jugar con un jugador aleatorio" << endl;
    cout << "2. Jugar con un amigo" << endl;
    cout << "3. Salir" << endl;

    int opcionRival;

    do {
        cout << "\nIntroduce una opcion: ";
        cin >> opcionRival;
        cin.ignore();

        switch (opcionRival) {
            case 1: {
                // OBTENER LISTA DE USUARIOS DESDE SERVIDOR 
                char sendBuff[512];
                char recvBuff[2048] = {0};

                strcpy(sendBuff, "GUU");
                send(*s, sendBuff, sizeof(sendBuff), 0);
                recv(*s, recvBuff, sizeof(recvBuff), 0);

                int numUsuarios = contarSlash(recvBuff);
                if (numUsuarios == 0) {
                    cout << "No hay usuarios disponibles." << endl;
                    break;
                }

                // Parseamos usuarios (cada usuario separado por '/')
                char tempBuff[2048];
                strcpy(tempBuff, recvBuff);
                char* tokens[100];
                int n = 0;

                char* token = strtok(tempBuff, "/");
                while (token != nullptr && n < 100) {
                    tokens[n++] = token;
                    token = strtok(nullptr, "/");
                }

                // Elegimos usuario aleatorio
                srand((unsigned int)time(NULL));
                int aleatorio = rand() % n;

                // Sacamos nombre (primer campo antes de ; )
                char copia[256];
                strncpy(copia, tokens[aleatorio], sizeof(copia));
                copia[sizeof(copia) - 1] = '\0';

                char* campo = strtok(copia, ";");
                char nombre[50] = {0};
                if (campo != nullptr) {
                    strncpy(nombre, campo, sizeof(nombre));
                }

                cout << "Jugarás contra el jugador " << "<" << nombre << "> " << endl;

                // Aquí podrías llamar a la función que inicia la partida, por ejemplo:
                // partidaDamas(s, logger);
                break;
            }

            case 2: {
                // BUSCAR AMIGO POR NOMBRE
                char sendBuff[512];
                char recvBuff[2048] = {0};
                strcpy(sendBuff, "GUU");
                send(*s, sendBuff, sizeof(sendBuff), 0);
                recv(*s, recvBuff, sizeof(recvBuff), 0);

                int numUsuarios = contarSlash(recvBuff);
                if (numUsuarios == 0) {
                    cout << "No hay usuarios disponibles." << endl;
                    break;
                }

                // Parseamos usuarios
                char tempBuff[2048];
                strcpy(tempBuff, recvBuff);
                char* tokens[100];
                int n = 0;

                char* token = strtok(tempBuff, "/");
                while (token != nullptr && n < 100) {
                    tokens[n++] = token;
                    token = strtok(nullptr, "/");
                }

                char nombreBuscado[50];
                cout << "Introduce el nombre del amigo: ";
                cin >> nombreBuscado;

                bool encontrado = false;
                for (int i = 0; i < n; ++i) {
                    char copia[256];
                    strncpy(copia, tokens[i], sizeof(copia));
                    copia[sizeof(copia) - 1] = '\0';

                    char* campo = strtok(copia, ";");
                    if (campo != nullptr && strcmp(campo, nombreBuscado) == 0) {
                        encontrado = true;
                        break;
                    }
                }

                if (encontrado) {
                    cout << "Jugador <" << nombreBuscado << "> encontrado. Iniciando partida..." << endl;
                    // Aquí iría llamada a iniciar partida con ese jugador
                } else {
                    cout << "Jugador no encontrado." << endl;
                }

                break;
            }

            case 3:
                // Volver al menú principal
                menuPaginaPrincipal(s, logger);
                return;  // Salir para no continuar el loop

            default:
                cout << "Opción no válida. Intenta de nuevo." << endl;
        }
    } while (true);
}

//----------------------------------------------------------------------------------------
void menuPartidasDisponibles(SOCKET *s, Log &logger)
{
	system("cls");
	cout << "*********************************************************************" << endl
		 << "************************PARTIDAS DISPONIBLES*************************" << endl
		 << "*********************************************************************" << endl
		 << endl;
	cout << "Give us a moment..." << endl;

	if (modoJuegoSeleccionado == 1) {
		char sendBuff[512];
		char recvBuff[1024];
		recvBuff[0] = '\0';

		strcpy(sendBuff, "GPA::Damas");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		recv(*s, recvBuff, sizeof(recvBuff), 0);

		// Crear una copia para no modificar recvBuff original
		char tempBuff[1024];
		strcpy(tempBuff, recvBuff);
		
		// Lista auxiliar
		const int MAX_PARTIDAS = 50;
		char* listaPartidas[MAX_PARTIDAS];
		int numPartidas = 0;

		// Menter partidas a la lista auxiliar
		char* token = strtok(tempBuff, "/");
		while (token != nullptr && numPartidas < MAX_PARTIDAS) {
			listaPartidas[numPartidas++] = token;
			token = strtok(nullptr, "/");
		}

		// Recorrer lista auxiliar
		for (int j = 0; j < numPartidas; ++j) {
			char temp[256];
			strncpy(temp, listaPartidas[j], sizeof(temp));
			temp[sizeof(temp) - 1] = '\0';

			char* campo = strtok(temp, ";");
			char codigo[10] = {0};
			char juego[20] = {0};
			char fecha[20] = {0};

			int i = 0;
			while (campo != nullptr) {
				if (i == 0) strncpy(codigo, campo, sizeof(codigo));
				else if (i == 1) strncpy(juego, campo, sizeof(juego));
				else if (i == 2) strncpy(fecha, campo, sizeof(fecha));
				i++;
				campo = strtok(nullptr, ";");
			}

			if (i == 3) {
				cout << j + 1 << ". " << juego << ": " << codigo << " (" << fecha << ")" << endl;
			}
		}

		cout << "\nPulsa ENTER para volver...";
		cin.ignore();
		cin.get();

	} else if (modoJuegoSeleccionado == 2) {
		char sendBuff[512];
		char recvBuff[1024];
		recvBuff[0] = '\0';

		strcpy(sendBuff, "GPA::4enRaya");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		recv(*s, recvBuff, sizeof(recvBuff), 0);

		// Crear una copia para no modificar recvBuff original
		char tempBuff[1024];
		strcpy(tempBuff, recvBuff);
		
		// Lista auxiliar
		const int MAX_PARTIDAS = contarSlash(recvBuff);
		char* listaPartidas[MAX_PARTIDAS];
		int numPartidas = 0;

		// Menter partidas a la lista auxiliar
		char* token = strtok(tempBuff, "/");
		while (token != nullptr && numPartidas < MAX_PARTIDAS) {
			listaPartidas[numPartidas++] = token;
			token = strtok(nullptr, "/");
		}

		// Recorrer lista auxiliar
		for (int j = 0; j < numPartidas; ++j) {
			char temp[256];
			strncpy(temp, listaPartidas[j], sizeof(temp));
			temp[sizeof(temp) - 1] = '\0';

			char* campo = strtok(temp, ";");
			char codigo[5];
			char juego[6];
			char fecha[11];

			int i = 0;
			while (campo != nullptr) {
				if (i == 0) strcpy(codigo, campo);
				else if (i == 1) strncpy(juego, campo, sizeof(juego));
				else if (i == 2) strncpy(fecha, campo, sizeof(fecha));
				i++;
				campo = strtok(nullptr, ";");
			}

			if (i == 3) {
				cout << j + 1 << ". " << juego << ": " << codigo << " (" << fecha << ")" << endl;
			}
		}

		cout << "\nPulsa ENTER para volver...";
		cin.ignore();
		cin.get();
	} else {
		cout << modoJuegoSeleccionado;
	}
}
void menuUnirsePartida(SOCKET *s, Log &logger)
{
	system("cls");
	cout << "*********************************************************************" << endl
		 << "************************UNIRSE A UNA PARTIDA*************************" << endl
		 << "*********************************************************************" << endl
		 << endl;

	if (modoJuegoSeleccionado == 1) {
		char sendBuff[512];
		char recvBuff[1024] = {0};

		strcpy(sendBuff, "GPA::Damas");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		recv(*s, recvBuff, sizeof(recvBuff), 0);

		int max = contarSlash(recvBuff);

		char** codes = getCodigoPartidas(recvBuff,max);

		cout << "Codigos disponibles:" << endl;
		for (int i = 0; i < max; ++i) {
			cout << codes[i] << endl;
		}

		char code[5];
		cout << "\nInsertar codigo: ";
		cin >> code;

		bool encontrado = false;
		for (int i = 0; i < max; ++i) {
			if (strcmp(code, codes[i]) == 0) {
				encontrado = true;
				break;
			}
		}

		if (encontrado) {
			printf("Sending message 1... \n");
		strcpy(sendBuff, "BEGINDAMAS");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		Sleep(3000);
		printf("Receiving message 1... \n");
		recv(*s, recvBuff, sizeof(recvBuff), 0);
		recvBuff[0]='\0';
		printf("Data received: %s \n", recvBuff);

		char input[256];
		do
		{
		cin >> input;
		printf("Sending message ... \n");
		strcpy(sendBuff, input);
		input[0]='\0';
		send(*s, sendBuff, sizeof(sendBuff), 0);
		printf("Data sent: %s \n", sendBuff);
		
		printf("Receiving message ... \n");
		recv(*s, recvBuff, sizeof(recvBuff), 0);
		printf("Data received: %s \n", recvBuff);
		} while (strcmp(input, "Bye"));
			}else {
		cout << "Code doesn't exist." << endl;
			}

		// Liberar memoria
		for (int i = 0; i < max; ++i) {
			delete[] codes[i];
		}
		delete[] codes;
	}
}
void menuTorneosDisponibles(SOCKET *s, Log &logger)
{
	system("cls");
	cout << "*********************************************************************" << endl
		 << "************************TORNEOS DISPONIBLES*************************" << endl
		 << "*********************************************************************" << endl
		 << endl;
	cout << "Give us a moment..." << endl;

	if (modoJuegoSeleccionado == 1) {
		char sendBuff[512];
		char recvBuff[1024];
		recvBuff[0] = '\0';

		strcpy(sendBuff, "GTO");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		recv(*s, recvBuff, sizeof(recvBuff), 0);

		// Crear una copia para no modificar recvBuff original
		char tempBuff[1024];
		strcpy(tempBuff, recvBuff);
		//cout << tempBuff;
		// Lista auxiliar
		const int MAX_TORNEOS = 50;
		char* listaPartidas[MAX_TORNEOS];
		int numTorneos = 0;

		// Menter partidas a la lista auxiliar
		char* token = strtok(tempBuff, "/");
		while (token != nullptr && numTorneos < MAX_TORNEOS) {
			listaPartidas[numTorneos++] = token;
			token = strtok(nullptr, "/");
		}

		// Recorrer lista auxiliar
		for (int j = 0; j < numTorneos; ++j) {
			char temp[256];
			strncpy(temp, listaPartidas[j], sizeof(temp));
			temp[sizeof(temp) - 1] = '\0';

			char* campo = strtok(temp, ";");
			char codigo[10] = {0};
			char nombre[20] = {0};
			char fecha1[20] = {0};
			char fecha2[20] = {0};

			int i = 0;
			while (campo != nullptr) {
				if (i == 0) strncpy(codigo, campo, sizeof(codigo));
				else if (i == 1) strncpy(fecha1, campo, sizeof(fecha1));
				else if (i == 2) strncpy(fecha2, campo, sizeof(fecha2));
				else if (i == 3) strncpy(nombre, campo, sizeof(nombre));
				i++;
				campo = strtok(nullptr, ";");
			}

			if (i == 4) {
				cout << j + 1 << ". " << codigo << ": " << nombre << " (" << fecha1 << " - " << fecha2 << ")" << endl;
			}
		}

		cout << "\nPulsa ENTER para volver...";
		cin.ignore();
		cin.get();

	}else {
		cout << modoJuegoSeleccionado;
	}
}

void menuUnirseTorneo(SOCKET *s, Log &logger)
{
	system("cls");
	cout << "*********************************************************************" << endl
		 << "************************UNIRSE A UNA PARTIDA*************************" << endl
		 << "*********************************************************************" << endl
		 << endl;

	if (modoJuegoSeleccionado == 1) {
		char sendBuff[512];
		char recvBuff[1024] = {0};

		strcpy(sendBuff, "GTO");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		recv(*s, recvBuff, sizeof(recvBuff), 0);

		int max = contarSlash(recvBuff);

		char** codes = getCodigoPartidas(recvBuff,max);

		cout << "Codigos disponibles:" << endl;
		for (int i = 0; i < max; ++i) {
			cout << codes[i] << endl;
		}

		char code[5];
		cout << "\nInsertar codigo: ";
		cin >> code;

		bool encontrado = false;
		for (int i = 0; i < max; ++i) {
			if (strcmp(code, codes[i]) == 0) {
				encontrado = true;
				break;
			}
		}

		if (encontrado) {
			printf("Sending message 1... \n");
		strcpy(sendBuff, "BEGINDAMAS");
		send(*s, sendBuff, sizeof(sendBuff), 0);
		Sleep(3000);
		printf("Receiving message 1... \n");
		recv(*s, recvBuff, sizeof(recvBuff), 0);
		recvBuff[0]='\0';
		printf("Data received: %s \n", recvBuff);

		char input[256];
		do
		{
		cin >> input;
		printf("Sending message ... \n");
		strcpy(sendBuff, input);
		input[0]='\0';
		send(*s, sendBuff, sizeof(sendBuff), 0);
		printf("Data sent: %s \n", sendBuff);
		
		printf("Receiving message ... \n");
		recv(*s, recvBuff, sizeof(recvBuff), 0);
		printf("Data received: %s \n", recvBuff);
		} while (strcmp(input, "Bye"));
			}else {
		cout << "Code doesn't exist." << endl;
			}

		// Liberar memoria
		for (int i = 0; i < max; ++i) {
			delete[] codes[i];
		}
		delete[] codes;
	}
}
//--------------------------------------------SOCKET---------------------------------------------------------------

//comando salir
void enviarComandoSalir(SOCKET *s) {
	char sendBuff[512];
	strcpy(sendBuff, "Bye");
	send(*s, sendBuff, sizeof(sendBuff), 0);
}

//comando iniciar sesion
int enviarComandoIniciarSesion(SOCKET* s, char* email,char* contrasena){
	char sendBuff[512], recvBuff[1024];

	strcpy(sendBuff, "COMP_INICIO_SESION");

	strcpy(sendBuff,"GUS::");
	strcat(sendBuff,email);
	send(*s, sendBuff, sizeof(sendBuff), 0);
	recv(*s, recvBuff, sizeof(recvBuff), 0);
	int i;
	cout << recvBuff << endl;
	if(!strcmp(recvBuff,contrasena)){
		i=1;
	}else{
		i=0;
	}

	return i;

}

//comando registro 
void enviarComandoRegistro(SOCKET* s, Usuario& u) {
	char sendBuff[512];
	char recvBuff[1024];
	//strcpy(sendBuff, "COMP_REGISTRO");
	//send(*s, sendBuff, sizeof(sendBuff), 0);
	sendBuff[0]='\0';
    strcpy(sendBuff,"REG::");
    strcat(sendBuff,u.getNombre());
    strcat(sendBuff,";");
    strcat(sendBuff,u.getEmail());
    strcat(sendBuff,";");
    strcat(sendBuff,u.getContrasenya());
    strcat(sendBuff,"\0");
    cout << sendBuff;
    send(*s, sendBuff, 512, 0);
	recv(*s, recvBuff,1024,0);
}
