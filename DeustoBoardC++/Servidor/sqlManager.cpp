
extern "C" {
#include "sqlite3.h"
}


#include <windows.h>
#include <string.h>
#include <iostream>

#include "sqlManager.h"
#include "usuario.h"
#include "fichero.h"

using namespace std;

sqlite3* abrirDB() {
	sqlite3 *db;

	Fichero f;
	f.leerConfig();

	int result = sqlite3_open(f.getDatabase(), &db);
	if (result != SQLITE_OK) {
		cout << "Error opening database" << endl;
	}

	return db;
}

int inicioSesion(char dni[], char contrasena[], Usuario& u) {
	sqlite3 *db = abrirDB();
	int result = 0;

	sqlite3_stmt *stmt;

	char sql[] = "SELECT * FROM Usuario WHERE email = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}

	result = sqlite3_step(stmt);
	if (result == SQLITE_ROW) {
		if (strcmp(dni, (char*) sqlite3_column_text(stmt, 0)) == 0
				&& strcmp(contrasena, (char*) sqlite3_column_text(stmt, 6)) == 0) {


			u.setEmail((char*) sqlite3_column_text(stmt, 0));
			u.setNombre((char*) sqlite3_column_text(stmt, 1));
			u.setContrasenya((char*) sqlite3_column_text(stmt, 2));


			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return 1;
		}

	} else if (result != SQLITE_DONE) {
		printf("Error fetching data\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 0;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int anadirUsuario(Usuario u) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;


	char sql[] = "SELECT * FROM Usuario WHERE email = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, u.getEmail(), strlen(u.getEmail()), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);

	if (result == SQLITE_ROW) {
		printf("El usuario ya existe");
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 0;
	} else {
		char sql2[] = "INSERT INTO Usuario VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		int result = sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt,
				NULL);
		if (result != SQLITE_OK) {
			printf("Error preparing statement\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_bind_text(stmt, 1, u.getEmail(), strlen(u.getEmail()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 2, u.getNombre(), strlen(u.getNombre()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 3, u.getConstrasenya(), strlen(u.getConstrasenya()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}



		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			printf("Error inserting new data into Usuario\n");
			return result;
		}
		cout << "Usuario con email " << u.getEmail() << " anadido correctamente" << endl;
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 0;
	}
}