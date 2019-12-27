#include "database.h"
#include "text.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <sqlite3.h>

using namespace std;

int         db_callbackAns;
int         db_id;
string      db_username;
string      db_password;

bool createDatabasePosts()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_POSTS_ERROR

    // Visibility - 0 (private) / 1 (public)

    char const *sql =   "DROP TABLE IF EXISTS Posts;" 
                        "CREATE TABLE Posts(PostId INT, PostText TEXT, OwnerId INT, PostVisibility INT);" 
                        "INSERT INTO Posts VALUES(1, 'Postare Cosmin', 2, 1);" 
                        "INSERT INTO Posts VALUES(2, 'Postare Cosmin', 2, 0);" 
                        "INSERT INTO Posts VALUES(3, 'Postare Andrei', 1, 0);" 
                        "INSERT INTO Posts VALUES(4, 'Postare Georgiana', 8, 1);" 
                        "INSERT INTO Posts VALUES(5, 'Postare Razvan', 7, 0);" 
                        "INSERT INTO Posts VALUES(6, 'Postare Cosmin', 2, 0);" 
                        "INSERT INTO Posts VALUES(7, 'Postare Andrei', 1, 1);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_POST_ERROR

    sqlite3_close(db);

    DB_CREATE_POSTS_OK

    return true;
}

bool createDatabaseUsers()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    char const *sql =   "DROP TABLE IF EXISTS Users;" 
                        "CREATE TABLE Users(UserId INT, UserName TEXT, UserPass TEXT, UserAdmin INT, UserOnline INT);" 
                        "INSERT INTO Users VALUES(1, 'Andrei', 'pass', 1, 0);" 
                        "INSERT INTO Users VALUES(2, 'Cosmin', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(3, 'Ion', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(4, 'Ana', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(5, 'Maria', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(6, 'Radu', 'pass', 0, 0);" 
                        "INSERT INTO Users VALUES(7, 'Razvan', 'pass', 0, 0);"
                        "INSERT INTO Users VALUES(8, 'Georgiana', 'pass', 0, 0);";
        
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

    ok = ok & createDatabaseUsers();
    ok = ok & createDatabasePosts();

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

    int db_nextId = computeNextId(1);

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

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_INSERT_USER_ERROR

    sqlite3_close(db);

    DB_INSERT_USER_OK

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

int computeNextId(int tabel)
{
    db_callbackAns = 0;

    string table = "";

    if(tabel == 1) table += "Users";
    if(tabel == 2) table += "Posts";

    string idName = "";

    if(tabel == 1) idName += "UserId";
    if(tabel == 2) idName += "PostId";

    string sql = "";
    sql += "SELECT MAX(";
    sql += idName;
    sql += ") FROM ";
    sql += table;
    sql += ";";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1)
            DB_OPEN_USER_ERROR
        if(tabel == 2)
            DB_OPEN_POSTS_ERROR
    }

    exitCode = sqlite3_exec(db, sql.c_str(), callbackGetMaxId, 0, &err);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1)
            DB_SELECT_USER_ERROR
        if(tabel == 2)
            DB_SELECT_POSTS_ERROR
    }

    sqlite3_close(db);

    return 1 + db_callbackAns;
}

bool existsId(int id, int tabel)
{
    db_callbackAns = 0;

    string table = "";

    if(tabel == 1) table += " Users ";
    if(tabel == 2) table += " Posts ";

    string idName = "";

    if(tabel == 1) idName += " UserId ";
    if(tabel == 2) idName += " PostId ";

    string sql = "";
    sql += "SELECT * FROM";
    sql += table;
    sql += "WHERE";
    sql += idName;
    sql += "= ";
    sql += std::to_string(id);
    sql += ";";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1)
            DB_OPEN_USER_ERROR
        if(tabel == 2)
            DB_OPEN_POSTS_ERROR
    }

    exitCode = sqlite3_exec(db, sql.c_str(), callbackCheckIfExists, 0, &err);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1)
            DB_SELECT_USER_ERROR
        if(tabel == 2)
            DB_SELECT_POSTS_ERROR
    }

    sqlite3_close(db);

    return (db_callbackAns == 1);
}

bool updateOn(int userid, int value)
{
    if(!existsId(userid, 1))
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

    exitCode = sqlite3_open("mydatabase.db", &db);

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

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), callbackCheckIfExists, 0, &err);

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

int callbackCheckIfExists(void *NotUsed, int argc, char **argv, 
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

int callbackGetMaxId(void *NotUsed, int argc, char **argv, 
                    char **azColName) {
    
    NotUsed = 0;

    printf("argc = %d\n\n", argc);

    if(argc)
    {
        db_callbackAns = atoi(argv[0]);
    }

    return 0;
}