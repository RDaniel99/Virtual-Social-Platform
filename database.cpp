#include "database.h"
#include "text.h"
#include <cstdio>
#include <iostream>
#include <sqlite3.h>

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
                        "INSERT INTO Users VALUES(7, 'Hummer', 'pass', 0);" ;
                        "INSERT INTO Users VALUES(8, 'Volkswagen', 'pass', 0);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_USER_ERROR

    sqlite3_close(db);

    DB_CREATE_USER_OK

    return true;
}

bool createDatabases()
{
    bool ok = true;

    ok = ok & createDatabaseUser();

    return ok;
}