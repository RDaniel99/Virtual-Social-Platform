#include "database.h"
#include "text.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <vector>
#include <sqlite3.h>

using namespace std;

struct userinfo
{
    int userId;
    string userName;
};

struct postinfo
{
    int postId;
    string postText;
    int postAuthor;
};

vector<postinfo>    db_posts;
vector<userinfo>    db_users;
int                 db_callbackAns;

// Users Table
string              db_username;
string              db_password;
int                 db_isAdmin;
int                 db_isOnline;
int                 db_isPrivate;

int                 db_id;
// Posts Table
string              db_posttext;
int                 db_authorid;
int                 db_visibility;

//  Friendship Table
int                 db_friendId1;
int                 db_friendId2;
int                 db_typefriendship;
int                 db_isaccepted;

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
                        "CREATE TABLE Users(UserId INT, UserName TEXT, UserPass TEXT, UserAdmin INT, UserOnline INT, UserPrivacy INT);" 
                        "INSERT INTO Users VALUES(1, 'Andrei', 'pass', 1, 0, 0);" 
                        "INSERT INTO Users VALUES(2, 'Cosmin', 'pass', 0, 0, 0);" 
                        "INSERT INTO Users VALUES(3, 'Ion', 'pass', 0, 0, 0);" 
                        "INSERT INTO Users VALUES(4, 'Ana', 'pass', 0, 0, 0);" 
                        "INSERT INTO Users VALUES(5, 'Maria', 'pass', 0, 0, 0);" 
                        "INSERT INTO Users VALUES(6, 'Radu', 'pass', 0, 0, 0);" 
                        "INSERT INTO Users VALUES(7, 'Razvan', 'pass', 0, 0, 0);"
                        "INSERT INTO Users VALUES(8, 'Georgiana', 'pass', 0, 0, 0);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_USER_ERROR

    sqlite3_close(db);

    DB_CREATE_USER_OK

    return true;
}

bool createDatabaseFriendships()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    char const *sql =   "DROP TABLE IF EXISTS Friendships;" 
                        "CREATE TABLE Friendships(IDSender INT, IdReciever INT, TypeFriends INT, IsAccepted INT);" 
                        "INSERT INTO Friendships VALUES(1, 4, 1, 1);" 
                        "INSERT INTO Friendships VALUES(2, 1, 2, 0);" 
                        "INSERT INTO Friendships VALUES(2, 4, 2, 1);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    DB_CREATE_FRIENDSHIPS_OK

    return true;
}

bool createDatabases()
{
    bool ok = true;

    ok = ok & createDatabaseUsers();
    ok = ok & createDatabasePosts();
    ok = ok & createDatabaseFriendships();

    return ok;
}

bool updateUser(char *nume, char *pass, int privacy, int userid)
{
    bool ok = true;
    
    ok = ok & validateName(nume);
    ok = ok & validatePass(pass);
    ok = ok & validatePrivacy(privacy);

    if(!ok)
        return false;

    string sql = "UPDATE Users SET UserName = '";
    sql += nume;
    sql += "', UserPass = '";
    sql += pass;
    sql += "', UserPrivacy = ";
    sql += std::to_string(privacy);
    sql += " WHERE UserId = ";
    sql += std::to_string(userid);
    sql += ";";

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

    DB_UPDATE_USER_OK

    return true;
}

bool insertUser(char *nume, char* pass, int isAdmin, int privacy)
{
    bool ok = true;
    
    ok = ok & validateName(nume);
    ok = ok & validatePass(pass);
    ok = ok & validateAdmin(isAdmin);
    ok = ok & validatePrivacy(privacy);

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
    sql += ", 0, ";
    sql += (privacy ? '1' : '0');
    sql += ");";
    
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

bool getOnline(char *msg)
{
    db_users.clear();

    string sql = "";
    sql += "SELECT * FROM Users WHERE UserOnline = 1 AND UserPrivacy = 0;";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_USER_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), callbackCollectOnline, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_USER_ERROR

    sqlite3_close(db);

    strcpy(msg, "");
    if(db_users.size() == 0)
        return false;

    string ans = "";
    for(unsigned int i = 0; i < db_users.size(); i++)
    {
        string toAdd = "";
        toAdd += "Name: ";
        toAdd += db_users[i].userName;
        toAdd += " | ID: ";
        toAdd += std::to_string(db_users[i].userId);

        ans += toAdd;
        ans += "\n";
    }

    strcpy(msg, ans.c_str());

    return true;
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
    if(id == -1)
        return false;

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

bool validatePrivacy(int privacy)
{
    return (privacy == 0 || privacy == 1);
}

bool areFriends(int id1, int id2, int accepted)
{
    db_callbackAns = 0;

    if(id1 == -1 || id2 == -1)
        return false;

    string sql = "";
    sql += "SELECT * FROM Friendships WHERE ";
    sql += "IDSender = ";
    sql += std::to_string(id1);
    sql += " AND IdReciever = ";
    sql += std::to_string(id2);
    sql += " AND IsAccepted = ";
    sql += std::to_string(accepted);

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    db_friendId2 = db_friendId1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), callbackAreInFriends, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    return db_callbackAns && accepted == db_isaccepted;
}

bool validatePostText(char *postText)
{
    if(strlen(postText) > 0 && strlen(postText) <= 100)
        return true;

    return false;
}

bool validateOwnerId(int ownerid)
{
    return existsId(ownerid, 1);
}

bool validateVisibility(int visibility)
{
    return (0 <= visibility && visibility <= 2);
}

bool addPost(char *postText, int ownerid, int visibility)
{
    bool ok = true;

    ok = ok & validatePostText(postText);
    ok = ok & validateOwnerId(ownerid);
    ok = ok & validateVisibility(visibility);

    if(!ok)
        return false;

    int idPost = computeNextId(2);

    string sql = "";
    sql += "INSERT INTO Posts VALUES(";
    sql += std::to_string(idPost);
    sql += ", '";
    sql += postText;
    sql += "', ";
    sql += std::to_string(ownerid);
    sql += ", ";
    sql += std::to_string(visibility);
    sql += ");";
    
    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_POSTS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_POSTS_ERROR

    sqlite3_close(db);

    return true;
}

bool editPost(int postid, char *postText, int ownerid, int visibility)
{
    bool ok = true;

    ok = ok & validatePostText(postText);
    ok = ok & validateOwnerId(ownerid);
    ok = ok & validateVisibility(visibility);

    if(!ok)
        return false;

    char tmp[1000];
    getPosts(ownerid, tmp, true);

    ok = false;
    for(int i = 0; i < db_posts.size(); i++)
    {
        if(db_posts[i].postId == postid && db_posts[i].postAuthor == ownerid)
        {
            ok = true;
            break;
        }
    }

    if(!ok)
        return false;

    string sql = "";
    sql += "UPDATE Posts SET PostText = '";
    sql += postText;
    sql += "', PostVisibility = ";
    sql += std::to_string(visibility);
    sql += " WHERE PostId = ";
    sql += std::to_string(postid);
    sql += " AND OwnerId = ";
    sql += std::to_string(ownerid);
    sql += ";";
    
    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_POSTS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_POSTS_ERROR

    sqlite3_close(db);

    return true;
}

bool deletePost(int postid, int userid)
{
    bool ok = true;

    ok = ok & existsId(userid, 1);
    ok = ok & existsId(postid, 2);
    
    if(!ok)
        return false;
    
    ok = false;
    ok = ok | (userid == db_authorid);
    ok = ok | isUserAdmin(userid);

    if(!ok)
        return false;

    string sql = "";
    sql += "DELETE FROM Posts WHERE PostId = ";
    sql += std::to_string(postid);
    sql += ";";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_POSTS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_DELETE_POST_ERROR

    sqlite3_close(db);

    return true;
}

bool isUserAdmin(int userid)
{
    if(!existsId(userid, 1))
        return false;

    return db_isAdmin;
}

bool getPosts(int userid, char *msg, bool areAll)
{
    db_posts.clear();

    db_id = userid;

    string sql = "";
    sql += "SELECT * FROM Posts";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_POSTS_ERROR

    if(!areAll)
        exitCode = sqlite3_exec(db, sql.c_str(), callbackCollectPosts, 0, &err);
    else
        exitCode = sqlite3_exec(db, sql.c_str(), callbackCollectAllPosts, 0, &err);
    

    if(exitCode != SQLITE_OK)
        DB_SELECT_POSTS_ERROR

    sqlite3_close(db);

    if(db_posts.size() == 0)
        return false;
    
    string ans = "";
    for(unsigned int i = 0; i < db_posts.size(); i++)
    {
        string toAppend = "";
        toAppend += "PostId: ";
        toAppend += std::to_string(db_posts[i].postId);
        toAppend += "| ";

        toAppend += "Message: ";
        toAppend += db_posts[i].postText;
        toAppend += "| ";
        
        toAppend += "AuthorId: ";
        toAppend += std::to_string(db_posts[i].postAuthor);
        toAppend += ";\n";

        ans += toAppend;
    }

    strcpy(msg, ans.c_str());

    return true;
}

int callbackCheckIfExists(void *NotUsed, int argc, char **argv, char **azColName) 
{
    
    NotUsed = 0;
    
    if(argc)
    {
        db_callbackAns = 1;

        if(argc == 6)
        {
            db_username = "";
            db_password = "";
            db_id = -1;
            db_isAdmin = 0;
            db_isOnline = 0;
            db_isPrivate = 0;

            db_id = atoi(argv[0]);
            db_username += argv[1];
            db_password += argv[2];
            db_isAdmin = atoi(argv[3]);
            db_isOnline = atoi(argv[4]);
            db_isPrivate = atoi(argv[5]);
        }

        if(argc == 4)
        {
            db_id = -1;
            db_posttext = "";
            db_authorid = -1;
            db_visibility = 0;

            db_id = atoi(argv[0]);
            db_authorid = atoi(argv[2]);
            db_posttext += argv[1];
            db_visibility = atoi(argv[3]);
        }
    }

    return 0;
}

int callbackGetMaxId(void *NotUsed, int argc, char **argv, char **azColName) 
{    
    NotUsed = 0;

    if(argc)
    {
        db_callbackAns = atoi(argv[0]);
    }

    return 0;
}

bool isPostValid(char *authorid, char *visibility)
{
    if(atoi(authorid) == db_id)
        return true;

    if(atoi(visibility) == 0)
        return true;

    if(isUserAdmin(db_id))
        return true;

    if(areFriends(atoi(authorid), db_id, 1))
        if(db_typefriendship >= atoi(visibility))
            return true;

    if(areFriends(db_id, atoi(authorid), 1))
        if(db_typefriendship >= atoi(visibility))
            return true;

    return false;
}

int callbackCollectPosts(void *NotUsed, int argc, char **argv, char **azColName) 
{
    NotUsed = 0;
        
    if(isPostValid(argv[2], argv[3]))
    {
        postinfo toAdd;
        toAdd.postId = atoi(argv[0]);
        toAdd.postText = "";
        toAdd.postText += argv[1];
        toAdd.postAuthor = atoi(argv[2]);

        db_posts.push_back(toAdd);
    }

    return 0;
}

int callbackAreInFriends(void *NotUsed, int argc, char **argv, char **azColName) 
{    
    db_callbackAns = 1;

    if(argc)
    {
        db_friendId1 = atoi(argv[0]);
        db_friendId2 = atoi(argv[1]);
        db_typefriendship = atoi(argv[2]);
        db_isaccepted = atoi(argv[3]);
    }

    return 0;
}                

int callbackCollectOnline(void *NotUsed, int argc, char **argv, char **azColName) 
{
    if(argc == 6)
    {
        userinfo toAdd;
        toAdd.userId = atoi(argv[0]);
        toAdd.userName = "";
        toAdd.userName += argv[1];

        db_users.push_back(toAdd);
    }

    return 0;
}

int callbackCollectAllPosts(void *NotUsed, int argc, char **argv, char **azColName) 
{
    NotUsed = 0;
    
    postinfo toAdd;
    toAdd.postId = atoi(argv[0]);
    toAdd.postText = "";
    toAdd.postText += argv[1];
    toAdd.postAuthor = atoi(argv[2]);

    db_posts.push_back(toAdd);

    return 0;
}