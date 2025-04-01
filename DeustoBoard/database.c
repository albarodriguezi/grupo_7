#include <stdio.h>
#include <stdlib.h>
#include "usuario.h"
#include "partida.h"
#include "sqlite3.h"
#include <string.h>
#include "database.h"
#include <time.h>

int lineasFichero(char* f)
{
	FILE *file = fopen(f, "r");
    char c;
	int num_lines = 0;
	//abrir fichero para lectura
	//leer mientras no se llegue al final del fichero EOF
	while ((c = fgetc(file)) != EOF)
	{
		if (c == '\n')
			num_lines++;  
		//putchar(c);
	}
	//cerrar fichero
	fclose(file);
	return num_lines;
}


Usuario* getListaUsuario(){
    sqlite3 *db;
	sqlite3_stmt *stmt;
    char sql2[] = "select * from USUARIO;COMMIT";
    char sql3[] = "select count(*) from USUARIO;COMMIT";
    sqlite3_open("database.sqlite", &db);
    
	sqlite3_prepare_v2(db, sql3, strlen(sql3), &stmt, NULL) ;
    sqlite3_step(stmt);
	printf("\n");
    int count = sqlite3_column_int(stmt, 0);
    Usuario* userList = (Usuario*)malloc((count) * sizeof(Usuario));
    sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;
    int result;
    int i = 0;
	do {
        result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
            strcpy(userList[i].email,(char*) sqlite3_column_text(stmt, 0));
            strcpy(userList[i].nombreUsuario,(char*) sqlite3_column_text(stmt, 1));
            strcpy(userList[i].contrasenya,(char*) sqlite3_column_text(stmt, 2));
			//printf("%s\n", userList[i].email);
            i++;
		}
	} while (result == SQLITE_ROW);
	//printf("\n");
    //printf("%s\n",userList[49].email);
	sqlite3_finalize(stmt);

	/* --- SELECT (fin) --- */

	sqlite3_close(db);

    return userList;
}

Partida* getListaPartida(){
    sqlite3 *db;
	sqlite3_stmt *stmt;
    char sql2[] = "select * from PARTIDA;COMMIT";
    char sql3[] = "select count(*) from PARTIDA;COMMIT";
    sqlite3_open("database.sqlite", &db);
    
	sqlite3_prepare_v2(db, sql3, strlen(sql3), &stmt, NULL) ;
    sqlite3_step(stmt);
	printf("\n");
    int count = sqlite3_column_int(stmt, 0);
    Partida* PartList = (Partida*)malloc((count) * sizeof(Partida));
    sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;
    int result;
    int i = 0;
	do {
        result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
            strcpy(PartList[i].codigo,(char*) sqlite3_column_text(stmt, 0));
            PartList[i].resultado=(int) sqlite3_column_int(stmt, 3);
            strcpy(PartList[i].juego,(char*) sqlite3_column_text(stmt, 2));
            const char *dateStr = (const char *)sqlite3_column_text(stmt, 4);
            sscanf(dateStr, "%d-%d-%d", &PartList[i].fecha.tm_year, &PartList[i].fecha.tm_mon, &PartList[i].fecha.tm_mday);
            PartList[i].fecha.tm_year-=1900;
            PartList[i].fecha.tm_mon-=1;
            strcpy(PartList[i].codigotorneo,(char*) sqlite3_column_text(stmt, 1));
			//printf("%s\n", userList[i].email);
            i++;
		}
	} while (result == SQLITE_ROW);
	//printf("\n");
    //printf("%s\n",userList[49].email);
	sqlite3_finalize(stmt);

	/* --- SELECT (fin) --- */

	sqlite3_close(db);

    return PartList;
}
/*
void csvToDatabase(){
    FILE *file = fopen("users.csv","r");
    char ch;
    int csv_inx=0;
    char line[1024]; // Buffer to store a line
    int person_count = 1;
    Usuario* users=NULL;



    if (!file) {
        perror("Unable to open file");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        printf("%i\n",person_count);
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");

            users = malloc(person_count*sizeof(Usuario));
            while (token != NULL) {
            switch (csv_inx) {
                case 0:
                    strcpy(users[person_count].email, token);
                    break;
                case 1:
                    strcpy(users[person_count].nombreUsuario, token);
                    break;
                case 2:
                    strcpy(users[person_count].contrasenya, token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            csv_inx++;
        }

        person_count++;  // Increment the number of users read
    }
    printf("%s",users[30].email);
    free(users);
    fclose(file);
}

*/

Usuario getUsuario(char *email)
{
    Usuario u;
    sqlite3 *db;
	sqlite3_stmt *stmt;
    char sql2[] = "select * from USUARIO where EMAIL = ?;COMMIT";
    sqlite3_open("database.sqlite", &db);
    sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;
    sqlite3_bind_text(stmt, 1, email, strlen(email), SQLITE_TRANSIENT);
    int result;
    int i = 0;
	do {
        result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
            strcpy(u.email,(char*) sqlite3_column_text(stmt, 0));
            strcpy(u.nombreUsuario,(char*) sqlite3_column_text(stmt, 1));
            strcpy(u.contrasenya,(char*) sqlite3_column_text(stmt, 2));
			//printf("%s\n", u.email);
            }
            i++;
		
	} while (result == SQLITE_ROW);
	//printf("\n");
	sqlite3_finalize(stmt);
	sqlite3_close(db);
    return u;
}
Partida getPartida(char *codigo)
{
    Partida p;
    sqlite3 *db;
	sqlite3_stmt *stmt;
    char sql2[] = "select * from PARTIDA where CODPARTIDA = ?;COMMIT";
    sqlite3_open("database.sqlite", &db);
    sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;
    sqlite3_bind_text(stmt, 1, codigo, strlen(codigo), SQLITE_TRANSIENT);
    int result;
    int i = 0;
	do {
        result = sqlite3_step(stmt);
        
		if (result == SQLITE_ROW) {
            strcpy(p.codigo,(char*) sqlite3_column_text(stmt, 0));
            p.resultado=(int) sqlite3_column_int(stmt, 3);
            strcpy(p.juego,(char*) sqlite3_column_text(stmt, 2));
            const char *dateStr = (const char *)sqlite3_column_text(stmt, 4);
            //printf("%s\n", p.codigo);
            sscanf(dateStr, "%d-%d-%d", &p.fecha.tm_year, &p.fecha.tm_mon, &p.fecha.tm_mday);
            p.fecha.tm_year-=1900;
            p.fecha.tm_mon-=1;
            //strcpy(p.fecha,(struct tm) sqlite3_column_text(stmt, 4));
            strcpy(p.codigotorneo,(char*) sqlite3_column_text(stmt, 1));
			
            }
            i++;
		
	} while (result == SQLITE_ROW);
	//printf("\n");
	sqlite3_finalize(stmt);

	sqlite3_close(db);
    return p;
}

void saveUsuario(Usuario u){
    sqlite3 *db;
	sqlite3_stmt *stmt;
    sqlite3_open("database.sqlite", &db);
    //sqlite3_busy_timeout(db, 10000); 
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", 0, 0, 0);
    int result;
    char sql1[] = "insert into USUARIO (EMAIL,USERNAME,PASSWORD) values (?, ?, ?);COMMIT";
	
        // Bind values to the SQL statement
        //printf("%s\n",users[50].email);
        sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, u.email, strlen(u.email), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, u.nombreUsuario, strlen(u.nombreUsuario), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, u.contrasenya, strlen(u.contrasenya), SQLITE_TRANSIENT);

        // Execute the SQL statement
        
        result = sqlite3_step(stmt);
        printf("%i\n",result);
        if (result != SQLITE_DONE) {
            printf("Error inserting user %s: %s\n", u.nombreUsuario, sqlite3_errmsg(db));
        } else {
            printf("Usuario insertado: %s\n", u.nombreUsuario);
        }
        sqlite3_reset(stmt);
        
	
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
void savePartida(Partida p)
{
}
void createDB()
{
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

	sqlite3_open("database.sqlite", &db);

    char sql1[] = "CREATE TABLE IF NOT EXISTS USUARIO(EMAIL TEXT PRIMARY KEY NOT NULL,USERNAME TEXT NOT NULL,PASSWORD TEXT);COMMIT";
    char sql2[] = "CREATE TABLE IF NOT EXISTS PARTIDA(CODPARTIDA TEXT PRIMARY KEY NOT NULL,CODTORNEO TEXT NOT NULL,JUEGO TEXT,RESULTADO INT,FECHA DATE);COMMIT";
	char sql3[] = "CREATE TABLE IF NOT EXISTS PARTICIPA(CODPARTIDA TEXT PRIMARY KEY NOT NULL,USERA TEXT NOT NULL,USERB TEXT);COMMIT";
    sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) ;

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}
    sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt, NULL) ;
    result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}
    sqlite3_prepare_v2(db, sql3, strlen(sql3) + 1, &stmt, NULL) ;
    result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}
    //else{
		//printf("Tabla  insertado\n");
	//}
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void deleteDB(){
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

	sqlite3_open("database.sqlite", &db);

    char sql1[] = "DROP TABLE IF EXISTS USUARIO;COMMIT";
    char sql2[] = "DROP TABLE IF EXISTS PARTIDA;COMMIT";
    char sql3[] = "DROP TABLE IF EXISTS PARTICIPA;COMMIT";

	sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) ;

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}else{
		printf("Tabla  insertado\n");
	}

    sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt, NULL) ;
    result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}else{
		printf("Tabla  insertado\n");
	}

    sqlite3_prepare_v2(db, sql3, strlen(sql3) + 1, &stmt, NULL) ;
    result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}else{
		printf("Tabla  insertado\n");
	}
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void csvToDatabaseUsuario() {
    
    char line[1024];          // Buffer to store a line
    int person_count = 0;     // Number of users read
    Usuario *users;    // Pointer to dynamically store users
    int num;
    num = lineasFichero("users.csv");
    //rewind(file);
    FILE *file = fopen("users.csv", "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }
     // Skip the first line (header)
     if (fgets(line, sizeof(line), file) == NULL) {
        printf("File is empty or unable to read the header.\n");
        fclose(file);
        return;
    }
    users = (Usuario*) malloc(num * sizeof(Usuario));

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline, if present
        line[strcspn(line, "\r\n")] = '\0';

        // Dynamically allocate memory for a new user
        
        if (!users) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }

        // Split the line into tokens and populate the struct
        char *token = strtok(line, ",");
        int csv_inx = 0;  // Reset csv_inx for each row

        while (token != NULL) {
            switch (csv_inx) {
                case 0:
                    strcpy(users[person_count].email, token);
                    break;
                case 1:
                    strcpy(users[person_count].nombreUsuario, token);
                    break;
                case 2:
                    strcpy(users[person_count].contrasenya, token);
                    //printf("%s\n",token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            csv_inx++;
            
        }
        
        person_count++;  // Increment the number of users read
        
    }
    //printf("%s\n",users[3].contrasenya);
    /*
    // Print a specific user's email for verification (e.g., user 30)
    if (person_count > 30) {
        printf("User 30's email: %s\n", users[30].email);
    } else {
        printf("Not enough users in the file to display user 30.\n");
    }
    */
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;
	sqlite3_open("database.sqlite", &db);
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", 0, 0, 0);
    char sql1[] = "insert into USUARIO (EMAIL,USERNAME,PASSWORD) values (?, ?, ?);COMMIT";
    int i = 0;
	
    for (int i = 0; i < num-1; i++) {
        // Bind values to the SQL statement
        //printf("%s\n",users[50].email);
        sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, users[i].email, strlen(users[i].email), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, users[i].nombreUsuario, strlen(users[i].nombreUsuario), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, users[i].contrasenya, strlen(users[i].contrasenya), SQLITE_TRANSIENT);

        // Execute the SQL statement
        
        result = sqlite3_step(stmt);
        //printf("%s\n",users[50].email);
        /*
        if (result != SQLITE_DONE) {
            printf("Error inserting user %s: %s\n", users[i].nombreUsuario, sqlite3_errmsg(db));
        } else {
            printf("Usuario insertado: %s\n", users[i].nombreUsuario);
        }
        */
        // Reset the statement to reuse it for the next user
        sqlite3_reset(stmt);
        
    }
	
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "PRAGMA journal_mode=DELETE;", 0, 0, 0);
    sqlite3_close(db);
	

    free(users);

    // Close the file
    fclose(file);
}

void csvToDatabasePartida() {
    
    char line[1024];          // Buffer to store a line
    int count = 0;     // Number of users read
    Partida *partidas;    // Pointer to dynamically store users
    int num;
    num = lineasFichero("partidas.csv");
    //rewind(file);
    FILE *file = fopen("partidas.csv", "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }
     // Skip the first line (header)
     if (fgets(line, sizeof(line), file) == NULL) {
        printf("File is empty or unable to read the header.\n");
        fclose(file);
        return;
    }
    partidas = (Partida*) malloc(num * sizeof(Partida));

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline, if present
        line[strcspn(line, "\r\n")] = '\0';

        // Dynamically allocate memory for a new user
        
        if (!partidas) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }

        // Split the line into tokens and populate the struct
        char *token = strtok(line, ",");
        int csv_inx = 0;  // Reset csv_inx for each row

        while (token != NULL) {
            switch (csv_inx) {
                case 0:
                    strcpy(partidas[count].codigo, token);
                    //printf("%s\n",token);
                    break;
                case 1:
                    sscanf(token,"%i",&partidas[count].resultado);
                    //printf("%i\n",partidas[count].resultado);
                    break;
                case 2:
                    strcpy(partidas[count].juego, token);
                    break;
                case 3:
                    sscanf(token,"%i",&partidas[count].fecha.tm_year);
                    partidas[count].fecha.tm_year-=1900;
                    //printf("%s\n",token);
                    break;
                case 4:
                    sscanf(token,"%i",&partidas[count].fecha.tm_mon);
                    partidas[count].fecha.tm_year-=1;
                    //printf("%s\n",token);
                    break;
                case 5:
                    sscanf(token,"%i",&partidas[count].fecha.tm_mday);
                    //partidas[count].fecha.tm_year-=1900;
                    //printf("%s\n",token);
                    break;
                case 6:
                    strcpy(partidas[count].codigotorneo, token);
                    //printf("%s\n",token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            csv_inx++;
            
        }
        
        count++;  // Increment the number of users read
        
    }
    //printf("%s\n",users[3].contrasenya);
    /*
    // Print a specific user's email for verification (e.g., user 30)
    if (person_count > 30) {
        printf("User 30's email: %s\n", users[30].email);
    } else {
        printf("Not enough users in the file to display user 30.\n");
    }
    */
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;
	sqlite3_open("database.sqlite", &db);

    char sql1[] = "insert into PARTIDA (CODPARTIDA,RESULTADO,JUEGO,FECHA,CODTORNEO) values (?, ?, ?, ?, ?);COMMIT";
    int i = 0;
	
    for (int i = 0; i < num-1; i++) {
        // Bind values to the SQL statement
        //printf("%s\n",users[50].email);
        int year = partidas[i].fecha.tm_year+1900;
        int month = partidas[i].fecha.tm_mon+1;
        int day = partidas[i].fecha.tm_mday;
        char cyear[5], cmonth[3], cday[3], date[11];
        snprintf(cyear, sizeof(cyear), "%d", year);
        snprintf(cmonth, sizeof(cmonth), "%02d", month);  // Ensures two digits (e.g., "03")
        snprintf(cday, sizeof(cday), "%02d", day);
        snprintf(date, sizeof(date), "%s-%s-%s", cyear, cmonth, cday);
        
        sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, partidas[i].codigo, strlen(partidas[i].codigo), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, partidas[i].resultado);
        sqlite3_bind_text(stmt, 3, partidas[i].juego, strlen(partidas[i].juego), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, date, strlen(date), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, partidas[i].codigotorneo, strlen(partidas[i].codigotorneo), SQLITE_TRANSIENT);

        // Execute the SQL statement
        
        result = sqlite3_step(stmt);
        //printf("%s\n",users[50].email);
        /*
        if (result != SQLITE_DONE) {
            printf("Error inserting user %s: %s\n", users[i].nombreUsuario, sqlite3_errmsg(db));
        } else {
            printf("Usuario insertado: %s\n", users[i].nombreUsuario);
        }
        */
        // Reset the statement to reuse it for the next user
        sqlite3_reset(stmt);
        
    }
	
    sqlite3_finalize(stmt);
    sqlite3_close(db);
	

    free(partidas);

    // Close the file
    fclose(file);
}

void csvToDatabaseParticipa() {
    
    char line[1024];          // Buffer to store a line
    int person_count = 0;     // Number of users read
    Usuario *users;    // Pointer to dynamically store users
    int num;
    num = lineasFichero("users.csv");
    //rewind(file);
    FILE *file = fopen("users.csv", "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }
     // Skip the first line (header)
     if (fgets(line, sizeof(line), file) == NULL) {
        printf("File is empty or unable to read the header.\n");
        fclose(file);
        return;
    }
    users = (Usuario*) malloc(num * sizeof(Usuario));

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline, if present
        line[strcspn(line, "\r\n")] = '\0';

        // Dynamically allocate memory for a new user
        
        if (!users) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }

        // Split the line into tokens and populate the struct
        char *token = strtok(line, ",");
        int csv_inx = 0;  // Reset csv_inx for each row

        while (token != NULL) {
            switch (csv_inx) {
                case 0:
                    strcpy(users[person_count].email, token);
                    break;
                case 1:
                    strcpy(users[person_count].nombreUsuario, token);
                    break;
                case 2:
                    strcpy(users[person_count].contrasenya, token);
                    //printf("%s\n",token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            csv_inx++;
            
        }
        
        person_count++;  // Increment the number of users read
        
    }
    //printf("%s\n",users[3].contrasenya);
    /*
    // Print a specific user's email for verification (e.g., user 30)
    if (person_count > 30) {
        printf("User 30's email: %s\n", users[30].email);
    } else {
        printf("Not enough users in the file to display user 30.\n");
    }
    */
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;
	sqlite3_open("database.sqlite", &db);

    char sql1[] = "insert into USUARIO (EMAIL,USERNAME,PASSWORD) values (?, ?, ?);COMMIT";
    int i = 0;
	
    for (int i = 0; i < num-1; i++) {
        // Bind values to the SQL statement
        //printf("%s\n",users[50].email);
        sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, users[i].email, strlen(users[i].email), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, users[i].nombreUsuario, strlen(users[i].nombreUsuario), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, users[i].contrasenya, strlen(users[i].contrasenya), SQLITE_TRANSIENT);

        // Execute the SQL statement
        
        result = sqlite3_step(stmt);
        //printf("%s\n",users[50].email);
        /*
        if (result != SQLITE_DONE) {
            printf("Error inserting user %s: %s\n", users[i].nombreUsuario, sqlite3_errmsg(db));
        } else {
            printf("Usuario insertado: %s\n", users[i].nombreUsuario);
        }
        */
        // Reset the statement to reuse it for the next user
        sqlite3_reset(stmt);
        
    }
	
    sqlite3_finalize(stmt);
    sqlite3_close(db);
	

    free(users);

    // Close the file
    fclose(file);
}