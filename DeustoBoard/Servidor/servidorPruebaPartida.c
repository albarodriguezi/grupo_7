// IMPORTANT: Winsock Library ("ws2_32") should be linked with -lws2_32

#include <stdio.h>
#include <winsock2.h>
#include "servidorPruebaPartida.h"
#include "servidorPruebaPartidaRaya.h"
#include "usuario.h"
#include "torneo.h"
#include "database.h"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main(int argc, char *argv[])
{

    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[1024], recvBuff[512];

    csvToDatabaseUsuario();
    csvToDatabasePartida();

    csvToDatabaseParticipa();

    csvToDatabaseTorneo();

    csvToDatabaseRegistrado();


    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }

    printf("Initialised.\n");

    // SOCKET creation
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // BIND (the IP/port with socket)
    if (bind(conn_socket, (struct sockaddr *)&server,
             sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Bind done.\n");

    // LISTEN to incoming connections (socket server moves to listening mode)
    if (listen(conn_socket, 1) == SOCKET_ERROR)
    {
        printf("Listen failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    // ACCEPT incoming connections (server keeps waiting for them)
    printf("Waiting for incoming connections...\n");
    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr *)&client, &stsize);
    // Using comm_socket is able to send/receive data to/from connected client
    if (comm_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }
    printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
           ntohs(client.sin_port));

    // Closing the listening sockets (is not going to be used anymore)
    closesocket(conn_socket);

    // SEND and RECEIVE data
    printf("Waiting for incoming messages from client... \n");
    do
    {
        recibirMensaje(recvBuff, comm_socket);
        printf("Sending reply... \n");
		strcpy(sendBuff, "ACK -> ");
		strcat(sendBuff, recvBuff);
        printf("2\n");
        char substring[11];
        strncpy(substring,recvBuff,10);
        substring[10] = '\0';
        char substringdos[12];
        strncpy(substringdos,recvBuff,11);
        substringdos[11] = '\0';
        printf(recvBuff);
        if (!strcmp(substring, "BEGINDAMAS"))
        {
            int tamano = getTamanoListaPartida("Damas");
            Partida * partidas = getListaPartidaJuego("Damas");
            bool encontrada = false;
            for (int i = 0; i < tamano; i++)
            {
                if (!strcmp(partidas[i].codigo, &recvBuff[10])){
                    printf(partidas[i].codigo);
                    partidaDamas(sendBuff, recvBuff, comm_socket, &(partidas[i]));
                    encontrada = true;
                break;
                }
            }
            if (!encontrada) {
            // Crear nueva partida
            Partida p;
            strncpy(p.codigo, &recvBuff[10], 4);
            p.codigo[4] = '\0';

            printf("Nueva partida creada: %s\n", p.codigo);
            partidaDamas(sendBuff, recvBuff, comm_socket, &p);


        }
            //p.
            break;
        }else if (!strcmp(substringdos, "BEGINCUATRO"))
        {
            int tamano = getTamanoListaPartida("4enRaya");
            Partida * partidas = getListaPartidaJuego("4enRaya");
            bool encontrada = false;
            for (int i = 0; i < tamano; i++)
            {
                if (!strcmp(partidas[i].codigo, &recvBuff[11])){
                    printf(partidas[i].codigo);
                    partidaCuatroRaya(sendBuff, recvBuff, comm_socket, &(partidas[i]));
                    encontrada = true;
                break;
                }

            }
            if (!encontrada) {
            // Crear nueva partida
            Partida p;
            strncpy(p.codigo, &recvBuff[10], 4);
            p.codigo[4] = '\0';

            printf("Nueva partida creada: %s\n", p.codigo);
            partidaCuatroRaya(sendBuff, recvBuff, comm_socket, &p);


            }
            sendBuff[0]='\0';
            //break;
        }else if(recvBuff[0]=='G'&&recvBuff[1]=='U'&&recvBuff[2]=='S'){ // Iniciar sesion
				Usuario u=getUsuario(&recvBuff[5]);
				printf("%s\n",u.contrasenya);
				strcpy(sendBuff,u.contrasenya);
		}else if(recvBuff[0]=='R'&&recvBuff[1]=='E'&&recvBuff[2]=='G'){ //Registrar usuario
				Usuario u;
				int i=0;
				char datos[155];
				strcpy(datos,&recvBuff[5]);
				char *token = strtok(datos, ";");
				while (token != NULL && i < 3) {
        			if (i == 0) {
            			strcpy(u.nombreUsuario, token);
        			} else if (i == 1) {
            			strcpy(u.email, token);
        			} else if (i == 2) {
            			strcpy(u.contrasenya, token);
        			}
        			i++;
        			token = strtok(NULL, ";");
    			}
				strcpy(sendBuff,u.nombreUsuario);
				saveUsuario(u);
			}else if(recvBuff[0]=='G'&&recvBuff[1]=='P'&&recvBuff[2]=='A'){  // Get partida for a game
				int tamano = getTamanoListaPartida(&recvBuff[5]);
				int i=0;
				Partida* ps=getListaPartidaJuego(&recvBuff[5]);
				sendBuff[0]='\0';
				while(i<tamano){
					strcat(sendBuff,ps[i].codigo);
					strcat(sendBuff,";");
					strcat(sendBuff,ps[i].juego);
					strcat(sendBuff,";");
					char dateStr[11];  
    				strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &ps[i].fecha);
					strcat(sendBuff,dateStr);
					strcat(sendBuff,"/");

					
					i++;
				}
                strcat(sendBuff,"\0");

			}else if(recvBuff[0]=='G'&&recvBuff[1]=='U'&&recvBuff[2]=='U'){  //Get lista usuario
				int tamano = getTamanoListaUsuario();
				int i=0;
				Usuario* us=getListaUsuario();
				sendBuff[0]='\0';
				
				while(i<tamano){
					strcat(sendBuff,us[i].email);
					strcat(sendBuff,";");
					strcat(sendBuff,us[i].nombreUsuario);
					strcat(sendBuff,";");
					strcat(sendBuff,us[i].contrasenya);
					strcat(sendBuff,"//");
					i++;
					printf("%i\n",i);
				}
				printf("%s\n",sendBuff);

			}else if(recvBuff[0]=='S'&&recvBuff[1]=='P'&&recvBuff[2]=='A'){ //SPA Insert Partida
				Partida p;  
				int i=0;
				char datos[155];
				strcpy(datos,&recvBuff[5]);
				char *token = strtok(datos, ";");
				while (token != NULL && i < 3) {
        			if (i == 0) {
            			strcpy(p.codigo, token);
        			} else if (i == 1) {
            			strcpy(p.juego, token);
        			} else if (i == 2) {
						sscanf(token, "%d-%d-%d", &p.fecha.tm_year, &p.fecha.tm_mon, &p.fecha.tm_mday);
    					p.fecha.tm_year -= 1900;  
    					p.fecha.tm_mon -= 1;
        			}
        			i++;
        			token = strtok(NULL, ";");
    			}
				strcpy(p.codigotorneo, "0000");
				p.resultado=0;
				strcpy(sendBuff,p.codigo);
				savePartida(p);
			}else if(recvBuff[0]=='G'&&recvBuff[1]=='T'&&recvBuff[2]=='O'){  // Get partida for a game
				int tamano = getTamanoListaTorneo();
				int i=0;
				Torneo* ts=getListaTorneo();
				sendBuff[0]='\0';
				while(i<tamano){
					strcat(sendBuff,ts[i].codt);
					strcat(sendBuff,";");
					strcat(sendBuff,ts[i].fechai);
					strcat(sendBuff,";");
					strcat(sendBuff,ts[i].fechaf);
					strcat(sendBuff,";");
					strcat(sendBuff,ts[i].nombret);
					strcat(sendBuff,"/");

					
					i++;
				}
                strcat(sendBuff,"\0");

			}else if(recvBuff[0]=='G'&&recvBuff[1]=='N'&&recvBuff[2]=='U'){  // Get partida for a game
				int tamano = getTamanoListaUsuario();
				int i=0;
				Usuario* us=getListaUsuario();
				sendBuff[0]='\0';
				
				while(i<tamano){
					strcat(sendBuff,us[i].nombreUsuario);
					strcat(sendBuff,"/");
					i++;
					printf("%i\n",i);
				}
				printf("%s\n",sendBuff);

			}
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            
        

        // strcat(sendBuff, "\nGameStart");
        // partidaDamas(sendBuff);
        // send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);

        if (strcmp(recvBuff, "Out") == 0)
            break;
    } while (1);

    // CLOSING the sockets and cleaning Winsock...
    closesocket(comm_socket);
    WSACleanup();

    return 0;
}

void enviarMensaje(char *recvBuff, char *sendBuff, char *mensaje, SOCKET comm_socket)
{
    printf("Sending reply... \n");
    strcpy(sendBuff, "ACK -> ");
    //printf("%s\n",sendBuff);
    printf("About to copy recive buff...\n");
    strcat(sendBuff, recvBuff);
    printf("About to copy the message...\n");
    strcat(sendBuff,"\n");
    strcat(sendBuff, mensaje);
    printf("Almost sent...\n");
    send(comm_socket, sendBuff, 1024, 0);
    printf("Sent!\n");
}

char *recibirMensaje(char *recvBuff, SOCKET comm_socket)
{
    do
    {
        int bytes = recv(comm_socket, recvBuff, 512, 0);
        if (bytes > 0)
        {
            printf("Recibiendo mensaje... \n");
            printf("Datos recibidos: %s \n", recvBuff);
            break;
        }
    } while (1);
    return recvBuff;
}

Tablero8x8 crearTableroDamas()
{
    // Número 1-----Dama blanca
    // Número 2-----Reina blanca
    // Número 3-----Dama negra
    // Número 4-----Reina negra
    Tablero8x8 tableroDamas;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i < 3)
            {
                if ((j + i + 1) % 2 == 0)
                {
                    tableroDamas.array[i][j] = 3;
                }
                else
                {
                    tableroDamas.array[i][j] = 0;
                }
            }
            else if (i > 4)
            {
                if ((j + i + 1) % 2 == 0)
                {
                    tableroDamas.array[i][j] = 1;
                }
                else
                {
                    tableroDamas.array[i][j] = 0;
                }
            }
            else
            {
                tableroDamas.array[i][j] = 0;
            }
        }
    }
    return tableroDamas;
}
void partidaDamas(char *sendBuff, char *recvBuff, SOCKET comm_socket, Partida* partida)
{
    char mensaje[800];
    strcpy(mensaje, "\nIniciando partida a las Damas...\n Envíe cualquier input para empezar\n");
    Tablero8x8 tableroDamas = crearTableroDamas();
    imprimirTableroDamas(tableroDamas, mensaje);
    enviarMensaje(recvBuff,sendBuff,mensaje, comm_socket);

    char *temp = recibirMensaje(recvBuff, comm_socket);
    if(!strcmp(temp, "Bye")){
        return;
    }
    int isGameOver = 0;
    int numDamasN = 12;
    int numDamasB = 12;
    int ficha[2];
    //Necesito este string para recoger el input
    char str[4];
    int movimiento;
    
    FILE * fichero = crearCSVPartida("partidas.csv");
    char nomlog[11];
    printf("A punto de crear Log\n");
    printf(partida->codigo);
    time_t ahora;
    struct tm fecha_actual;
    time(&ahora); // obtener tiempo actual en formato time_t
    fecha_actual = *localtime(&ahora);
    sprintf(nomlog,"LOG/LOG%i%i%i.log",fecha_actual.tm_year+1900,fecha_actual.tm_mon,fecha_actual.tm_mday);
    FILE * log = fopen(nomlog, "a");
    if(log!=NULL)
    printf("\nLog creado\n");
    fprintf(log, "PartidaLasDamas_%c%c%c%c\n",partida->codigo[0],partida->codigo[1],partida->codigo[2],partida->codigo[3]);
    fflush(log);
    almacenarDatosPartida(partida->codigo,0,partida->juego,partida->fecha,partida->codigotorneo,fichero);
    csvToDatabasePartida();
    
    //Bucle del juego
    do
    {
        turnoJugador(&tableroDamas, &str[4], movimiento,2, &numDamasB, &isGameOver, sendBuff, recvBuff, comm_socket, log);
        printf("Quedan %i fichas blancas\n",numDamasB);
        if (isGameOver)
        {
            break;
        }
        
        if (numDamasB <= 0)
        {
            isGameOver = 1;
            fprintf(log,"GANANNEGRAS\n");
            fflush(log);
            partida->resultado = 2;
            break;
        }
        
        turnoJugador(&tableroDamas, &str[4], movimiento,1, &numDamasN,&isGameOver, sendBuff, recvBuff, comm_socket, log);
        printf("Quedan %i fichas negras\n",numDamasN);
        if (isGameOver)
        {
            break;
        }
        if (numDamasN <= 0)
        {
            isGameOver = 1;
            fprintf(log,"GANANBLANCAS\n");
            fflush(log);
            partida->resultado = 1;
        }

        
    } while (isGameOver != 1);
    printf("Ha concluido la partida\n");
    fprintf(log,"FINPARTIDA\n");
    fflush(log);
    fclose(log);
    almacenarDatosPartida(partida->codigo,partida->resultado,partida->juego,partida->fecha,partida->codigotorneo,fichero);
    
}
void imprimirTableroDamas(Tablero8x8 tablero, char *sendBuff)
{
    for (int i = 0; i < 8; i++)
    {
        char buffer[2];
        sprintf(buffer, "%i", i+1);
        strcat(sendBuff, buffer);
        for (int j = 0; j < 8; j++)
        {
            switch (tablero.array[i][j])
            {
            case 0:
                if ((j + i + 1) % 2 == 0)
                {
                    strcat(sendBuff, ANSI_COLOR_RED "[ ]" ANSI_COLOR_RESET);
                }
                else
                {
                    strcat(sendBuff, "[ ]");
                }
                break;
            case 1:
                if ((j + i + 1) % 2 == 0)
                {
                    strcat(sendBuff, ANSI_COLOR_RED "[" ANSI_COLOR_RESET);
                    strcat(sendBuff, "B");
                    strcat(sendBuff, ANSI_COLOR_RED "]" ANSI_COLOR_RESET);
                }
                else
                {
                    strcat(sendBuff, "[B]");
                }
                break;
            case 2:
                strcat(sendBuff, "[DB]");
                break;
            case 3:
                if ((j + i + 1) % 2 == 0)
                {
                    strcat(sendBuff, ANSI_COLOR_RED "[N]" ANSI_COLOR_RESET);
                }
                else
                {
                    strcat(sendBuff, "[");
                    strcat(sendBuff, ANSI_COLOR_RED "N" ANSI_COLOR_RESET);
                    strcat(sendBuff, "]");
                }
                break;
            case 4:
                if ((j + i + 1) % 2 == 0)
                {
                    strcat(sendBuff, ANSI_COLOR_RED "[DN]" ANSI_COLOR_RESET);
                }
                else
                {
                    strcat(sendBuff, "[");
                    strcat(sendBuff, ANSI_COLOR_RED "DN" ANSI_COLOR_RESET);
                    strcat(sendBuff, "]");
                }
                break;
            default:
                break;
            }
        }
        strcat(sendBuff, "\n");
    }
    strcat(sendBuff, "  1  2  3  4  5  6  7  8\n");
}

void imprimirTableroDamasconSeleccion(Tablero8x8 tablero, int fila, int columna, char *sendBuff){
    int isSelected = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {if (i== fila && j == columna)
        {
            isSelected = 1;
            strcat(sendBuff, ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);
        }
        
            switch (tablero.array[i][j])
            {
            case 0:
                strcat(sendBuff, "[ ]");
                break;
            case 1:
            strcat(sendBuff, "[B]");
            break;
            case 2:
            strcat(sendBuff, "[DB]");
            break;
            case 3:
            strcat(sendBuff, "[N]");
            break;
            case 4:
            strcat(sendBuff,"[DN]");
            break;
            default:
                break;
            }
            if (isSelected == 1)
            {
                strcat(sendBuff, ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);
                isSelected = 0;
            }
            
        }
        strcat(sendBuff, "\n");
    }
}

void turnoJugador(Tablero8x8 *tableroDamas, char str[4], int movimiento, int numJugador, int *piezasAdversario, int *isGameOver, char *sendBuff, char *recvBuff, SOCKET comm_socket,  FILE * log)
{
    int ficha[2];
    int dama;
    int damaReina;
    int sentidoMovimiento;
    char mensaje[700];
    if (numJugador == 1)
    {
        dama = 1;
        damaReina = 2;
        sentidoMovimiento = 1;
    }
    else
    {
        dama = 3;
        damaReina = 4;
        sentidoMovimiento = -1;
    }
    int contadorPosiblesCapturas = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tableroDamas->array[i][j] == dama || tableroDamas->array[i][j] == damaReina)
            {
                if (hayCapturaDisponible(*tableroDamas, numJugador, (tableroDamas->array[i][j] == damaReina), i, j))
                {
                    contadorPosiblesCapturas += 1;
                }
            }
        }
    }
    if (contadorPosiblesCapturas > 0)
    {
        int **fichasConPosibleCaptura = (int **)malloc(sizeof(int *) * contadorPosiblesCapturas);
        contadorPosiblesCapturas = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (tableroDamas->array[i][j] == dama || tableroDamas->array[i][j] == damaReina)
                {
                    if (hayCapturaDisponible(*tableroDamas, numJugador, (tableroDamas->array[i][j] == damaReina), i, j))
                    {
                        int *fichaConPosibleCaptura = (int *)malloc(sizeof(int) * 2);
                        fichaConPosibleCaptura[0] = i;
                        fichaConPosibleCaptura[1] = j;
                        fichasConPosibleCaptura[contadorPosiblesCapturas] = fichaConPosibleCaptura;
                        contadorPosiblesCapturas += 1;
                    }
                }
            }
        }
        do
        {
            imprimirTableroDamas(*tableroDamas, mensaje);
            char buffer[75];
            sprintf(buffer, "El jugador %i solo puede coger una de las fichas con captura disponible\n", numJugador);
            strcat(mensaje, buffer);
            enviarMensaje(recvBuff, sendBuff, mensaje, comm_socket);
            strcpy(mensaje, "");
            Sleep(500);
            str = recibirMensaje(recvBuff, comm_socket);
            if(!strcmp(str, "Bye")){
                *isGameOver = 1;
                return;
            }
            ficha[0] = str[0] - 49;
            ficha[1] = str[2] - 49;
            //printf("%i\n", ficha[0]);
            //printf("%i\n", ficha[1]);
            //printf("%i,%i\n", dama, tableroDamas->array[ficha[0]][ficha[1]]);
        } while (!(str[1] == ',' && ficha[0] < 9 && ficha[0] >= 0 && ficha[1] < 9 && ficha[1] >= 0 && (tableroDamas->array[ficha[0]][ficha[1]] == dama || tableroDamas->array[ficha[0]][ficha[1]] == damaReina)) || fichaEnArray(fichasConPosibleCaptura, contadorPosiblesCapturas, ficha[0], ficha[1]) == 0);
        // Este if sirve para saber si la captura es hacia la izquierda o hacia la derecha.
        if (hayPiezaEnProximidad(*tableroDamas, numJugador, 0, ficha[0], ficha[1], 0) == 1)
        {
            tableroDamas->array[ficha[0] - sentidoMovimiento * 2][ficha[1] - 2] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            tableroDamas->array[ficha[0] - sentidoMovimiento][ficha[1] - 1] = 0;
            fprintf(log, "%Jugador %i: %i,%ix%i,%i\n", numJugador, ficha[0], ficha[1], ficha[0] - sentidoMovimiento * 2, ficha[1] - 2);
            fflush(log);
            if (ficha[0] == 7 || ficha[0] == 0)
            {
                tableroDamas->array[ficha[0] - sentidoMovimiento * 2][ficha[1] - 2] = damaReina;
            }
        }
        else
        {
            tableroDamas->array[ficha[0] - sentidoMovimiento * 2][ficha[1] + 2] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            tableroDamas->array[ficha[0] - sentidoMovimiento][ficha[1] + 1] = 0;
            fprintf(log, "%Jugador %i: %i,%ix%i,%i\n", numJugador, ficha[0], ficha[1], ficha[0] - sentidoMovimiento * 2, ficha[1] + 2);
            fflush(log);
            if (ficha[0] - sentidoMovimiento * 2 == 7 || ficha[0] - sentidoMovimiento * 2 == 0)
            {
                tableroDamas->array[ficha[0] - sentidoMovimiento * 2][ficha[1] + 2] = damaReina;
                fprintf(log, "Jugador %i obtiene reina\n", numJugador);
                fflush(log);
            }
        }
        *piezasAdversario -= 1;
        imprimirTableroDamas(*tableroDamas, mensaje);

        // Libero memoria dinámica ocupada por array
        for (int i = 0; i < contadorPosiblesCapturas; i++)
        {
            free(fichasConPosibleCaptura[i]);
            fichasConPosibleCaptura[i] = NULL;
        }
        free(fichasConPosibleCaptura);
        fichasConPosibleCaptura = NULL;
    }
    else
    {

        // Bucle del input del jugador (hasta que input sea válido)
        do
        {
        labelFicha:
        strcpy(mensaje, "");
        imprimirTableroDamas(*tableroDamas, mensaje);
            if (numJugador == 1)
            {
                char buffer[86];
                sprintf(buffer, "Jugador %i, escoja una ficha valida (Blancas) para mover (Formato <<fila,columna>>)\n", numJugador);
                strcat(mensaje, buffer);
                enviarMensaje(recvBuff, sendBuff, mensaje, comm_socket );
            }
            else
            {
                char buffer[85];
                sprintf(buffer, "Jugador %i, escoja una ficha valida (Negras) para mover (Formato <<fila,columna>>)\n", numJugador);
                strcat(mensaje, buffer);
                enviarMensaje(recvBuff, sendBuff, mensaje, comm_socket );
            }
            strcpy(mensaje, "");
            char *input = recibirMensaje(recvBuff,comm_socket);
            if(!strcmp(input, "Bye")){
                *isGameOver = 1;
                return;
            }
            imprimirTableroDamasEnServer(*tableroDamas);
            // Resto 49 para convertir número ASCI a numero normal (-48) y restar 1 (las casillas empiezan en 1,
            // pero los indices en 0) al mismo tiempo

            str[0] = input[0];
            str[1] = input[1];
            str[2] = input[2];
            
            ficha[0] = str[0] - 49;
            ficha[1] = str[2] - 49;
        } while (!((str[1] == ',') && ficha[0] < 9 && ficha[0] >= 0 && ficha[1] < 9 && ficha[1] >= 0 && (tableroDamas->array[ficha[0]][ficha[1]] == dama || tableroDamas->array[ficha[0]][ficha[1]] == damaReina)));
        imprimirTableroDamasconSeleccion(*tableroDamas, ficha[0], ficha[1], mensaje);
        int isdamaReina = 0;
    labelMovimiento:
        if (tableroDamas->array[ficha[0]][ficha[1]] == damaReina)
        {
            isdamaReina = 1;
            char buffer[152];
            sprintf(buffer, "Jugador %i, escoja un movimiento (1 Adelante Izquierda, 2 Adelante Derecha, 3 Atras Izquierda, 4 Atras Derecha,5 para volver a la seleccion de ficha)\n",numJugador);
            strcat(mensaje, buffer);
        }
        else
        {
            char buffer[99];
            sprintf(buffer, "Jugador %i, escoja un movimiento (1 Izquierda, 2 Derecha, 5 para volver a la seleccion de ficha)\n",numJugador);
            strcat(mensaje, buffer);
        }
        enviarMensaje(recvBuff, sendBuff, mensaje, comm_socket);
        strcpy(mensaje, "");
        printf("Right now mensaje is %i",strlen(mensaje));
        str = recibirMensaje(recvBuff,comm_socket);
        if(!strcmp(str, "Bye")){
            *isGameOver = 1;
            return;
        }
        movimiento = str[0] - 48;
        if ((ficha[1] == 0 && movimiento == 1) || (ficha[1] == 7 && movimiento == 2) || (((ficha[0] == 0 && sentidoMovimiento == 1 && isdamaReina == 0) || (ficha[0] == 7 && sentidoMovimiento == -1 && isdamaReina == 0)) && movimiento != 5))
        {
            printf("Este movimiento sacaria la ficha del tablero (invalido)\n");
            goto labelMovimiento;
        }

        switch (movimiento)
        {
        case 1:
            if (tableroDamas->array[ficha[0] - sentidoMovimiento][ficha[1] - 1] != 0)
            {
                strcat(mensaje, "No puedes ocupar el mismo espacio que otra ficha\n");
                goto labelMovimiento;
            }
            tableroDamas->array[ficha[0] - sentidoMovimiento][ficha[1] - 1] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            fprintf(log, "%Jugador %i: %i,%i->%i,%i\n", numJugador, ficha[0], ficha[1], ficha[0] - sentidoMovimiento, ficha[1] - 1);
            fflush(log);
            break;
        case 2:
            if (tableroDamas->array[ficha[0] - sentidoMovimiento][ficha[1] + 1] != 0)
            {
                strcat(mensaje, "No puedes ocupar el mismo espacio que otra ficha\n");
                imprimirTableroDamasconSeleccion(*tableroDamas, ficha[0], ficha[1], mensaje);
                goto labelMovimiento;
            }
            tableroDamas->array[ficha[0] - sentidoMovimiento][ficha[1] + 1] = tableroDamas->array[ficha[0]][ficha[1]];
            tableroDamas->array[ficha[0]][ficha[1]] = 0;
            fprintf(log, "%Jugador %i: %i,%i->%i,%i\n", numJugador, ficha[0], ficha[1], ficha[0] - sentidoMovimiento, ficha[1] + 1);
            fflush(log);
            break;
        case 3:
            if (isdamaReina)
            {
                if (tableroDamas->array[ficha[0] + sentidoMovimiento][ficha[1] - 1] != 0)
                {
                    strcat(mensaje,"No puedes ocupar el mismo espacio que otra ficha\n");
                    imprimirTableroDamasconSeleccion(*tableroDamas, ficha[0], ficha[1], mensaje);
                    goto labelMovimiento;
                }
                else
                {
                    tableroDamas->array[ficha[0] + sentidoMovimiento][ficha[1] - 1] = tableroDamas->array[ficha[0]][ficha[1]];
                    tableroDamas->array[ficha[0]][ficha[1]] = 0;
                    fprintf(log, "%Jugador %i: %i,%i->%i,%i\n", numJugador, ficha[0], ficha[1], ficha[0] + sentidoMovimiento, ficha[1] - 1);
                    fflush(log);
                }
            }
            else
            {
                goto labelMovimiento;
            }
            break;
        case 4:
            if (isdamaReina)
            {
                if (tableroDamas->array[ficha[0] + sentidoMovimiento][ficha[1] + 1] != 0)
                {
                    strcat(mensaje,"No puedes ocupar el mismo espacio que otra ficha\n");
                    imprimirTableroDamasconSeleccion(*tableroDamas, ficha[0], ficha[1], mensaje);
                    goto labelMovimiento;
                }
                else
                {
                    tableroDamas->array[ficha[0] + sentidoMovimiento][ficha[1] + 1] = tableroDamas->array[ficha[0]][ficha[1]];
                    tableroDamas->array[ficha[0]][ficha[1]] = 0;
                    fprintf(log, "%Jugador %i: %i,%i->%i,%i\n", numJugador, ficha[0], ficha[1], ficha[0] + sentidoMovimiento, ficha[1] + 1);
                    fflush(log);
                }
            }
            else
            {
                goto labelMovimiento;
            }
            break;

        case 5:
            goto labelFicha;
            break;
        default:
            goto labelMovimiento;
            break;
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (i == (numJugador - 1) * 7 && tableroDamas->array[i][j] == dama)
                {
                    tableroDamas->array[i][j] = damaReina;
                    char buffer[46];
                    sprintf(buffer, "¡El jugador %i dispone de una nueva reina!\n", numJugador);
                    strcat(mensaje, buffer);
                    fprintf(log, "Jugador %i obtiene reina\n", numJugador);
                    fflush(log);
                }
            }
        }
        imprimirTableroDamas(*tableroDamas, mensaje);
        printf("%i\n", strlen(mensaje));
        printf(mensaje);
        printf("%i\n", strlen(sendBuff));
        strcat(mensaje, "\nPulse Cualquier tecla para continuar\n");
        enviarMensaje(recvBuff,sendBuff,mensaje,comm_socket);
        strcpy(mensaje, "");
        char *str = recibirMensaje(recvBuff, comm_socket);
        if(!strcmp(str, "Bye")){
            *isGameOver = 1;
            return;
        }
    }
}
int hayPiezaEnProximidad(Tablero8x8 tableroDamas, int bando, int isReina, int filaFicha, int columnaFicha, int preferenciaOutOfBounds){
    int sentidoMovimiento;
    int outOfBoundsLeft = 0;
    int outOfBoundsRight = 0;
    int ajustadorComparacion;
    if (bando == 1)
    {
        sentidoMovimiento = 1;
    }else{
        sentidoMovimiento = -1;
    } if(preferenciaOutOfBounds != 0){
        ajustadorComparacion = 0;
        }else{
            ajustadorComparacion = 1;
        }
    if (!((columnaFicha-1)<(0+ajustadorComparacion) && filaFicha > 7*(bando-1)))
            {
        if ((preferenciaOutOfBounds != 2)&&(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] == 4+(sentidoMovimiento-1)))
        {
                return 1;
            }
        }else{outOfBoundsLeft = 1;
        }
        if (!((columnaFicha+1)>(7-ajustadorComparacion) && filaFicha > 7*(bando-1)))
            {
                if(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] == 4+(sentidoMovimiento-1)){
                return 2;
            }
        }else{outOfBoundsRight = 1;} if(!((columnaFicha-1)<(0+ajustadorComparacion)  && filaFicha > 7*(bando-1)))
            {
                if((preferenciaOutOfBounds != 2)&&(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha-1] != 0)){
                return 3;
            }
        }else{outOfBoundsLeft = 1;}if (!((columnaFicha+1)>(7-ajustadorComparacion) && filaFicha > 7*(bando-1)))
            {
                if(tableroDamas.array[filaFicha-sentidoMovimiento][columnaFicha+1] != 0){
                return 4;
            }
        }else{outOfBoundsRight = 1;
        }
        if (isReina == 1)
        {
            if (!((columnaFicha-1)<0+ajustadorComparacion))
            {
            if (tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] == 4+(sentidoMovimiento-1))
            {
                return 1;
            }
            }else{outOfBoundsLeft = 1;}if (!((columnaFicha+1)>7-ajustadorComparacion))
            {
                if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] == 3+(sentidoMovimiento-1) || tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] == 4+(sentidoMovimiento-1)){
                return 2;
            }
            }else{outOfBoundsRight = 1;}if (!((columnaFicha-1)<0+ajustadorComparacion))
            {
                if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha-1] != 0){
                return 3;
            }
            
            }else{outOfBoundsLeft = 1;}if (!((columnaFicha+1)>7-ajustadorComparacion))
            {
                if(tableroDamas.array[filaFicha+sentidoMovimiento][columnaFicha+1] != 0){
                return 4;
            }
        }else{outOfBoundsRight = 1;}
        }
        if ((preferenciaOutOfBounds == 1 && outOfBoundsLeft == 1) || (preferenciaOutOfBounds == 2 && outOfBoundsRight == 1) )
        {
            return 5;
        }
        return 0;     
}
int hayCapturaDisponible(Tablero8x8 tableroDamas, int bando, int isDama, int filaFicha, int columnaFicha){
    int sentidoMovimiento;
    if (bando == 1)
    {
        sentidoMovimiento = 1;
    }else{
        sentidoMovimiento = -1;
    }
    if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha,0) == 1)
    {
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha-1,1) != 5)
        {
            if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha-1,1) !=3 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha-1,1) !=1 ){
            return 1;
            }
            if (isDama)
            {
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha-1,1) == 5)
            {
                return 0;
            }
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha-1,1) != 3 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha-1,1) != 1){
                return 1;
            }
        }
    
        
        }
        
    }else if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha,columnaFicha,0) == 2){
        if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha+1,2) != 5)
        {
            if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha+1,2) != 4 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha-sentidoMovimiento,columnaFicha+1,2) != 2){
            return 1;
            }
            if (isDama)
            {
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha+1,2) == 5)
            {
                return 0;
            }
                if (hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha+1,2) != 4 && hayPiezaEnProximidad(tableroDamas,bando,isDama,filaFicha+sentidoMovimiento,columnaFicha+1,2) != 2){
                return 1;
                }
            }
        }
    }
    return 0;
    
}
int fichaEnArray(int** arrayFichas, int tamanyoArray, int filaFicha, int columnaFicha){
    for (int i = 0; i < tamanyoArray; i++)
    {
        if((arrayFichas[i][0] == filaFicha) && (arrayFichas[i][1] == columnaFicha) ){
            return 1;
        }
    }
    return 0;
    
}
void imprimirTableroDamasEnServer(Tablero8x8 tablero){
    for (int i = 0; i < 8; i++)
    {
        printf("%i",i+1);
        for (int j = 0; j < 8; j++)
        {
            switch (tablero.array[i][j])
            {
            case 0:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"[ ]"ANSI_COLOR_RESET);
            }else{
                printf("[ ]");
            }
                break;
            case 1:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"["ANSI_COLOR_RESET);
                printf("B");
                printf(ANSI_COLOR_RED"]"ANSI_COLOR_RESET);
            }else{
                printf("[B]");
            }
            break;
            case 2:
            printf("[DB]");
            break;
            case 3:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"[N]"ANSI_COLOR_RESET);
            }else{
                printf("[");
                printf(ANSI_COLOR_RED"N"ANSI_COLOR_RESET);
            printf("]");
            }
            break;
            case 4:
            if ((j+i+1)%2 == 0)
            {
                printf(ANSI_COLOR_RED"[DN]"ANSI_COLOR_RESET);
            }else{
                printf("[");
                printf(ANSI_COLOR_RED"DN"ANSI_COLOR_RESET);
            printf("]");
            }
            break;
            default:
                break;
            }
        }
        printf("\n");
    }
    printf("  1  2  3  4  5  6  7  8\n");
}
FILE* crearCSVPartida(char* fichero){
    FILE* fich = fopen(fichero, "w");
    return fich;
}
void almacenarDatosPartida(char codigo[5], int resultado, char juego[15], struct tm fecha,  char codigotorneo[5], FILE * fichero){
    
    fprintf(fichero,"codigoPartida,resultado,juego,anyo,mes,dia,codigoTorneo\n");
    if (fecha.tm_mday <10 && fecha.tm_mon <10)
    {
        fprintf(fichero, "%c%c%c%c,%i,%s,%i,0%i,0%i,%c%c%c%c\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }else if(fecha.tm_mday <10){
        fprintf(fichero, "%c%c%c%c,%i,%s,%i,%i,0%i,%c%c%c%c\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }else if(fecha.tm_mon <10){
        fprintf(fichero, "%c%c%c%c,%i,%s,%i,0%i,%i,%c%c%c%c\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }else{
        fprintf(fichero, "%c%c%c%c,%i,%s,%i,%i,%i,%c%c%c%c\n",codigo[0],codigo[1],codigo[2],codigo[3],resultado,juego,fecha.tm_year+1900,fecha.tm_mon,fecha.tm_mday, codigotorneo[0], codigotorneo[1], codigotorneo[2], codigotorneo[3]);
    }
    fclose(fichero);
    //csvToDatabasePartida();
    fichero = NULL;
}
