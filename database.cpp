#include "database.h"
#include "text.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <sqlite3.h>

using namespace std;

set<int>    db_usersIds;
int         db_nextId;
int         db_callbackAns;
int         db_id;
string      db_username;
string      db_password;

bool createDatabaseUser()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("users.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    char const *sql =   "DROP TABLE IF EXISTS Users;" 
                        "CREATE TABLE Users(UserId INT, UserName TEXT, UserPass TEXT, UserAdmin INT, UserOnline INT);" 
                        "INSERT INTO Users VALUES(1, 'Audi', 'pass', 1, 0);" 
                        "INSERT INTO Users VALUES(2, 'Mercedes', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(3, 'Skoda', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(4, 'Volvo', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(5, 'Bentley', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(6, 'Citroen', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(7, 'Hummer', 'pass', 0, 0);"
                        "INSERT INTO Users VALUES(8, 'Volkswagen', 'pass', 0, 0);";
        
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
    sql += ", 0);";
    
    DB_SQL_COMMAND

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

bool loginUser(char *nume, char* pass)
{
    bool ok = true;

    string pswd = "";
    string usr = "";
    usr += nume;
    pswd += pass;

    ok = ok & existsName(nume);
    ok = ok & (db_username == usr);
    ok = ok & (db_password == pswd);

    return ok;
}

int getIdFromLastSelect()
{
    return db_id;
}

bool updateOn(int userid, int value)
{
    if(db_usersIds.find(userid) == db_usersIds.end())
        return false;

    string sql = "";
    sql += "UPDATE Users SET UserOnline = ";
    sql += char(value + '0');
    sql += " WHERE UserId = ";
    sql += std::to_string(userid);
    sql += ';';

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("users.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_UPDATE_USER_ERROR

    sqlite3_close(db);

    return true;
}

bool existsName(char *nume)
{
    db_callbackAns = 0;

    string sql = "";
    sql += "SELECT * FROM Users WHERE UserName = ";
    sql += "'";
    sql += nume;
    sql += "';";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("users.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), callbackCheckIfNameExists, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_USER_ERROR

    sqlite3_close(db);

    return (db_callbackAns == 1);
}

bool validateName(char *nume)
{
    if(!nume || !strlen(nume))
        return false;

    return !existsName(nume);
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

int callbackCheckIfNameExists(void *NotUsed, int argc, char **argv, 
                    char **azColName) {
    
    NotUsed = 0;

    db_username = "";
    db_password = "";
    db_id = -1;
    
    if(argc)
    {
        db_callbackAns = 1;
        db_id = atoi(argv[0]);
        db_username += argv[1];
        db_password += argv[2];
    }

    return 0;
}