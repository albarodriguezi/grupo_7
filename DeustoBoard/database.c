#include <stdio.h>
#include <stdlib.h>
#include "usuario.h"
#include "sqlite3.h"
#include <string.h>
#include "database.h"

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
    char sql2[] = "select * from USUARIO";
    char sql3[] = "select count(*) from USUARIO";
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
    char sql2[] = "select * from USUARIO where EMAIL = ?";
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
void createDB(){
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

	sqlite3_open("database.sqlite", &db);

    char sql1[] = "CREATE TABLE IF NOT EXISTS USUARIO(EMAIL TEXT PRIMARY KEY NOT NULL,USERNAME TEXT NOT NULL,PASSWORD TEXT)";

	sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) ;

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}//else{
		//printf("Tabla  insertado\n");
	//}
    //sqlite3_finalize(db);
    sqlite3_close(db);
}

void deleteDB(){
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

	sqlite3_open("database.sqlite", &db);

    char sql1[] = "DROP TABLE IF EXISTS USUARIO";

	sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) ;

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error insertando tabla\n");
	}else{
		printf("Tabla  insertado\n");
	}
    //sqlite3_finalize(db);
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

    char sql1[] = "insert into USUARIO (EMAIL,USERNAME,PASSWORD) values (?, ?, ?)";
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
	
    //sqlite3_finalize(db);
    sqlite3_close(db);
	

    free(users);

    // Close the file
    fclose(file);
}

