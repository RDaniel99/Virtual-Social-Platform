#ifndef H_DATABASE
#define H_DATABASE

#include <set>

bool createDatabasePosts();
bool createDatabaseUsers();
bool createDatabases();

bool validateName(char *nume);
bool validatePass(char *pass);
bool validateAdmin(int isAdmin);
bool insertUser(char *nume, char* pass, int isAdmin = 0);
bool loginUser(char *nume, char* pass);
bool updateOn(int userid, int value);

bool existsId(int id, int tabel);
int computeNextId(int tabel);
int getIdFromLastSelect();
bool existsName(char *nume);
int callbackCheckIfExists(void *, int, char **, char **);
int callbackGetMaxId(void *, int, char **, char **);

#endif //H_DATABASE