#ifndef H_DATABASE
#define H_DATABASE

#include <set>

bool createDatabaseUser();
bool createDatabases();

bool validateName(char *nume);
bool validatePass(char *pass);
bool validateAdmin(int isAdmin);
bool insertUser(char *nume, char* pass, int isAdmin = 0);

int checkIfNameExists(void *, int, char **, char **);

#endif //H_DATABASE