#include "database.h"
#include "text.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <sqlite3.h>

using namespace std;

std::set<int>   db_usersIds;
int             db_nextId;
int             db_callbackAns;

bool createDatabaseUser()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("users.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    char const *sql =   "DROP TABLE IF EXISTS Users;" 
                        "CREATE TABLE Users(UserId INT, UserName TEXT, UserPass TEXT, UserAdmin INT);" 
                        "INSERT INTO Users VALUES(1, 'Audi', 'pass', 1);" 
                        "INSERT INTO Users VALUES(2, 'Mercedes', 'pass', 0);" 
                        "INSERT INTO Users VALUES(3, 'Skoda', 'pass', 0);" 
                        "INSERT INTO Users VALUES(4, 'Volvo', 'pass', 0);" 
                        "INSERT INTO Users VALUES(5, 'Bentley', 'pass', 0);" 
                        "INSERT INTO Users VALUES(6, 'Citroen', 'pass', 0);" 
                        "INSERT INTO Users VALUES(7, 'Hummer', 'pass', 0);"
                        "INSERT INTO Users VALUES(8, 'Volkswagen', 'pass', 0);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_USER_ERROR

    sqlite3_close(db);

    DB_CREATE_USER_OK

    for(int i = 1; i <= 8; i++)
        db_usersIds.insert(i);

    db_nextId = 9;

    return true;
}

bool createDatabases()
{
    bool ok = true;

    ok = ok & createDatabaseUser();

    return ok;
}

bool insertUser(char *nume, char* pass, int isAdmin)
{
    bool ok = true;
    db_callbackAns = 0;
    
    ok = ok & validateName(nume);
    ok = ok & validatePass(pass);
    ok = ok & validateAdmin(isAdmin);

    if(!ok)
        return false;

    string sql = "";
    sql += "INSERT INTO Users VALUES(";
    sql += std::to_string(db_nextId);
    sql += ", '";
    sql += nume;
    sql += "', '";
    sql += pass;
    sql += "', ";
    sql += (isAdmin ? '1' : '0');
    sql += ");";
    
    printf("SQL Command: %s\n", sql.c_str());

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("users.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_INSERT_USER_ERROR

    sqlite3_close(db);

    DB_INSERT_USER_OK
    db_usersIds.insert(db_nextId);
    db_nextId++;

    return true;
}

bool validateName(char *nume)
{
    if(!nume || !strlen(nume))
        return false;

    string sql = "";
    sql += "SELECT * FROM Users WHERE UserName = ";
    sql += "'";
    sql += nume;
    sql += "';";

    printf("SQL Command: %s\n", sql.c_str());

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("users.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), checkIfNameExists, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_USER_ERROR

    sqlite3_close(db);

    return (db_callbackAns == 0);
}

bool validatePass(char *pass)
{
    if(pass && strlen(pass) > 3)
        return true;
    
    return false;
}

bool validateAdmin(int isAdmin)
{
    if(isAdmin < 0 || isAdmin > 1)
        return false;
    
    return true;
}

int checkIfNameExists(void *NotUsed, int argc, char **argv, 
                    char **azColName) {
    
    NotUsed = 0;
    
    if(argc)
        db_callbackAns = 1;
        
    return 0;
}