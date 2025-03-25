#include <stdio.h>
#include <stdlib.h>
#include "usuario.h"
#include <string.h>


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


/*
void csvToDatabase() {
    FILE *file = fopen("users.csv", "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    char line[1024];          // Buffer to store a line
    int person_count = 0;     // Number of users read
    Usuario *users = NULL;    // Pointer to dynamically store users

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline, if present
        line[strcspn(line, "\n")] = '\0';

        // Dynamically allocate memory for a new user
        users = realloc(users, (person_count + 1) * sizeof(Usuario));
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
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            csv_inx++;
        }

        person_count++;  // Increment the number of users read
    }

    // Print a specific user's email for verification (e.g., user 30)
    if (person_count > 30) {
        printf("User 30's email: %s\n", users[30].email);
    } else {
        printf("Not enough users in the file to display user 30.\n");
    }

    // Free allocated memory
    free(users);

    // Close the file
    fclose(file);
}

*/