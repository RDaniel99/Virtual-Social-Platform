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

struct friendshipinfo
{
    int idSender;
    int idReciever;
    int type;
};

struct roominfo
{
    int roomId;
    string roomName;
    string msg;
    int userIdMsg;
};

vector<roominfo>        db_rooms;
vector<friendshipinfo>  db_friends;
vector<postinfo>        db_posts;
vector<userinfo>        db_users;
int                     db_callbackAns;

int                     db_tabelReq;
int                     db_id;      // between 1 and 3
int                     db_roomId;  // between 1 and 4

// Users Table (Table = 1)
string                  dbUser_Name;
string                  dbUser_Password;
int                     dbUser_Admin;
int                     dbUser_Online;
int                     dbUser_Private;

// Posts Table (Table = 2)
string                  dbPost_Text;
int                     dbPost_AuthorId;
int                     dbPost_Visibility;

//  Friendship Table (Table = 3)
int                     dbFriendship_Id1;
int                     dbFriendship_Id2;
int                     dbFriendship_Type;
int                     dbFriendship_Accepted;

// Room Table (Table = 4)
string                  dbRoom_Name;

// Messages Table (Table = 5)
int                     dbMessage_Id;
int                     dbMessage_RoomId;
int                     dbMessage_UserId;
string                  dbMessage_Text;


bool createDatabasePosts()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_POSTS_ERROR

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
                        "CREATE TABLE Users(UserId INT, UserName TEXT, UserPass TEXT, UserAdmin INT, UserOnline INT, UserPrivacy INT, UserRoom INT, UserLastMsg INT);" 
                        "INSERT INTO Users VALUES(1, 'Andrei', 'pass', 1, 0, 0, 0, 0);" 
                        "INSERT INTO Users VALUES(2, 'Cosmin', 'pass', 0, 0, 0, 0, 0);" 
                        "INSERT INTO Users VALUES(3, 'Ion', 'pass', 0, 0, 0, 0, 0);" 
                        "INSERT INTO Users VALUES(4, 'Ana', 'pass', 0, 0, 0, 0, 0);" 
                        "INSERT INTO Users VALUES(5, 'Maria', 'pass', 0, 0, 0, 0, 0);" 
                        "INSERT INTO Users VALUES(6, 'Radu', 'pass', 0, 0, 0, 0, 0);" 
                        "INSERT INTO Users VALUES(7, 'Razvan', 'pass', 0, 0, 0, 0, 0);"
                        "INSERT INTO Users VALUES(8, 'Georgiana', 'pass', 0, 0, 0, 0, 0);";
        
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
                        "CREATE TABLE Friendships(IdSender INT, IdReciever INT, TypeFriends INT, IsAccepted INT);" 
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

bool createDatabaseMessages()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_MESSAGES_ERROR

    char const *sql =   "DROP TABLE IF EXISTS Messages;" 
                        "CREATE TABLE Messages(MessageId INT, RoomId INT, IdUserMsg INT, Message TEXT);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_MESSAGES_ERROR

    sqlite3_close(db);

    DB_CREATE_MESSAGES_OK

    return true;
}

bool createDatabaseRooms()
{
    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_ROOMS_ERROR

    char const *sql =   "DROP TABLE IF EXISTS Rooms;" 
                        "CREATE TABLE Rooms(RoomId INT, RoomName TEXT);";
        
    exitCode = sqlite3_exec(db, sql, 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_CREATE_ROOMS_ERROR

    sqlite3_close(db);

    DB_CREATE_ROOMS_OK

    return true;
}

bool createDatabases()
{
    bool ok = true;

    ok = ok & createDatabaseUsers();
    ok = ok & createDatabasePosts();
    ok = ok & createDatabaseFriendships();
    ok = ok & createDatabaseRooms();
    ok = ok & createDatabaseMessages();

    return ok;
}

bool updateUser(char *nume, char *pass, int privacy, int userid)
{
    bool ok = true;
    
    ok = ok & validateName(nume);

    if(!ok && strcmp(nume, dbUser_Name.c_str()) == 0)
        ok = true;

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

bool addUser(char *nume, char* pass, int isAdmin, int privacy)
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
    sql += ", 0, 0);";
    
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

bool addRoom(int userid, char* numeRoom)
{
    if(!existsId(userid, 1))
        return false;

    if(db_roomId != 0)
        return false;

    if(existsName(numeRoom, 4))
        return false;

    int newId = computeNextId(4);

    string sql = "";
    sql += "INSERT INTO Rooms VALUES(";
    sql += std::to_string(newId);
    sql += ", '";
    sql += numeRoom;
    sql += "');";
    
    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_ROOMS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), callbackGetMaxId, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_INSERT_ROOMS_ERROR

    sqlite3_close(db);

    DB_INSERT_ROOMS_OK

    return true;
}

bool joinRoom(int userid, int roomid)
{
    if(!existsId(roomid, 4))
        return false;

    if(!existsId(userid, 1))
        return false;

    if(db_roomId)
        return false;

    string sql = "";
    sql += "UPDATE Users SET UserRoom = ";
    sql += std::to_string(roomid);
    sql += ", UserLastMsg = 0 WHERE UserId = ";
    sql += std::to_string(userid);

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

    int nextId = computeNextId(5);

    sql = "";
    sql += "INSERT INTO Messages VALUES (";
    sql += std::to_string(nextId);
    sql += ", ";
    sql += std::to_string(roomid);
    sql += ", ";
    sql += std::to_string(userid);
    sql += ", 'S-a alaturat ";
    sql += dbUser_Name;
    sql += "');";

    DB_SQL_COMMAND

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_INSERT_MESSAGES_ERROR

    sqlite3_close(db);

    return true;
}

bool deleteRoom(int userid, int roomid)
{
    printf("%d %d\n", userid, roomid);

    if(!existsId(roomid, 4))
        return false;

    if(!existsId(userid, 1))
        return false;

    if(!dbUser_Admin)
        return false;
    
    string sql = "";
    sql += "SELECT COUNT(*) FROM Users WHERE UserRoom = ";
    sql += std::to_string(roomid);
    sql += ";";
    
    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_ROOMS_ERROR

    db_callbackAns = 0;
    exitCode = sqlite3_exec(db, sql.c_str(), callbackGetMaxId, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_ROOMS_ERROR

    if(db_callbackAns)
        return false;

    sql = "";
    sql += "DELETE FROM Rooms WHERE RoomId = ";
    sql += std::to_string(roomid);
    sql += ";";

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_DELETE_ROOMS_ERROR

    sqlite3_close(db);

    return true;
}

bool leaveRoom(int userid)
{
    if(!existsId(userid, 1))
        return false;

    if(db_roomId == 0)
        return false;

    int tmp = db_roomId;

    string sql = "";
    sql += "UPDATE Users SET UserRoom = 0, UserLastMsg = 0 WHERE UserId = ";
    sql += std::to_string(userid);

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

    int nextId = computeNextId(5);

    sql = "";
    sql += "INSERT INTO Messages VALUES (";
    sql += std::to_string(nextId);
    sql += ", ";
    sql += std::to_string(tmp);
    sql += ", ";
    sql += std::to_string(userid);
    sql += ", 'A iesit ";
    sql += dbUser_Name;
    sql += ".');";

    DB_SQL_COMMAND

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_INSERT_MESSAGES_ERROR

    sqlite3_close(db);

    return true;
}

bool getMembers(int userid, char *msg)
{
    if(!existsId(userid, 1))
        return false;

    string sql = "";
    sql += "SELECT COUNT(*) FROM Users WHERE UserRoom = ";
    sql += std::to_string(db_roomId);
    sql += ";";
    
    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_ROOMS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), callbackGetMaxId, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_ROOMS_ERROR

    sqlite3_close(db);

    string ans = "";
    ans += DB_ROOM_MEMBERS;
    ans += std::to_string(db_callbackAns);

    strcpy(msg, "");
    strcpy(msg, ans.c_str());

    return true;
}

bool loginUser(char *nume, char* pass)
{
    bool ok = true;

    string pswd = "";
    string usr = "";
    usr += nume;
    pswd += pass;

    ok = ok & existsName(nume, 1);
    ok = ok & (dbUser_Name == usr);
    ok = ok & (dbUser_Password == pswd);
    ok = ok & (dbUser_Online == 0);

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

    db_tabelReq = 1;
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
    if(tabel == 4) table += "Rooms";
    if(tabel == 5) table += "Messages";

    string idName = "";

    if(tabel == 1) idName += "UserId";
    if(tabel == 2) idName += "PostId";
    if(tabel == 4) idName += "RoomId";
    if(tabel == 5) idName += "MessageId";

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
        if(tabel == 4)
            DB_OPEN_ROOMS_ERROR
        if(tabel == 5)
            DB_OPEN_MESSAGES_ERROR
    }

    exitCode = sqlite3_exec(db, sql.c_str(), callbackGetMaxId, 0, &err);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1)
            DB_SELECT_USER_ERROR
        if(tabel == 2)
            DB_SELECT_POSTS_ERROR
        if(tabel == 4)
            DB_SELECT_ROOMS_ERROR
        if(tabel == 5)
            DB_SELECT_MESSAGES_ERROR
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
    if(tabel == 4) table += " Rooms ";
    if(tabel == 5) table += " Messages ";

    string idName = "";

    if(tabel == 1) idName += " UserId ";
    if(tabel == 2) idName += " PostId ";
    if(tabel == 4) idName += " RoomId ";
    if(tabel == 5) idName += " MessageId ";

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
        if(tabel == 4)
            DB_OPEN_ROOMS_ERROR
        if(tabel == 5)
            DB_OPEN_MESSAGES_ERROR
    }

    db_tabelReq = tabel;
    exitCode = sqlite3_exec(db, sql.c_str(), callbackCheckIfExists, 0, &err);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1)
            DB_SELECT_USER_ERROR
        if(tabel == 2)
            DB_SELECT_POSTS_ERROR
        if(tabel == 4)
            DB_SELECT_ROOMS_ERROR
        if(tabel == 5)
            DB_SELECT_MESSAGES_ERROR
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

bool existsName(char *nume, int tabel)
{
    db_callbackAns = 0;

    string sql = "";
    if(tabel == 1)
    {
        sql += "SELECT * FROM Users WHERE UserName = ";
    }
    else 
    if(tabel == 4)
    {
        sql += "SELECT * FROM Rooms WHERE RoomName = ";
    }
    else
    {
        return false;
    }
    
    
    sql += "'";
    sql += nume;
    sql += "';";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1) DB_OPEN_USER_ERROR
        else DB_OPEN_ROOMS_ERROR
    }
    
    db_tabelReq = tabel;
    exitCode = sqlite3_exec(db, sql.c_str(), callbackCheckIfExists, 0, &err);

    if(exitCode != SQLITE_OK)
    {
        if(tabel == 1) DB_SELECT_USER_ERROR
        else DB_SELECT_ROOMS_ERROR
    }

    sqlite3_close(db);

    return (db_callbackAns == 1);
}

bool validateName(char *nume)
{
    if(!nume || !strlen(nume))
        return false;

    return !existsName(nume, 1);
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

    dbFriendship_Id2 = dbFriendship_Id1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    db_tabelReq = 3;
    exitCode = sqlite3_exec(db, sql.c_str(), callbackAreInFriends, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    return db_callbackAns && accepted == dbFriendship_Accepted;
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

bool updatePost(int postid, char *postText, int ownerid, int visibility)
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
    ok = ok | (userid == dbPost_AuthorId);
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

    return dbUser_Admin;
}

bool acceptReq(int userid, int friendid)
{
    bool ok = true;
    ok = ok & areFriends(friendid, userid, 0);

    if(!ok)
        return false;
    
    string sql = "";
    sql += "UPDATE Friendships SET IsAccepted = 1 WHERE IdSender = ";
    sql += std::to_string(friendid);
    sql += " AND IdReciever = ";
    sql += std::to_string(userid);
    sql += ";";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    dbFriendship_Id2 = dbFriendship_Id1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    return true;
}

bool addFriend(int senderId, int recieverId, int type)
{
    bool ok = true;
    ok = ok & existsId(recieverId, 1);
    ok = ok & (1 <= type && type <= 2);
    ok = ok & !areFriends(recieverId, senderId, 0);
    ok = ok & !areFriends(recieverId, senderId, 1);
    ok = ok & !areFriends(senderId, recieverId, 0);
    ok = ok & !areFriends(senderId, recieverId, 1);

    if(!ok)
        return false;

    string sql = "";
    sql += "INSERT INTO Friendships VALUES(";
    sql += to_string(senderId);
    sql += ", ";
    sql += to_string(recieverId);
    sql += ", ";
    sql += to_string(type);
    sql += ", 0)";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    dbFriendship_Id2 = dbFriendship_Id1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    return true;
}

bool deleteFriend(int userid, int friendid, int accepted)
{
    if(!areFriends(userid, friendid, accepted) && !areFriends(friendid, userid, accepted))
        return false;
    
    string sql = "";
    sql += "DELETE FROM Friendships WHERE (IdSender = ";
    sql += std::to_string(userid);
    sql += " AND IdReciever = ";
    sql += std::to_string(friendid);
    sql += ") OR (IdSender = ";
    sql += std::to_string(friendid);
    sql += " AND IdReciever = ";
    sql += std::to_string(userid);
    sql += ");";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    dbFriendship_Id2 = dbFriendship_Id1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    exitCode = sqlite3_exec(db, sql.c_str(), 0, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_DELETE_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    return true;
}

bool getName(int userid, char *msg)
{
    strcpy(msg, "");
    
    if(existsId(userid, 1))
        strcpy(msg, dbUser_Name.c_str());

    return strlen(msg) > 0;
}

bool getFriends(int userid, char *msg)
{
    db_friends.clear();

    if(!existsId(userid, 1))
        return false;
    
    string sql = "";
    sql += "SELECT * FROM Friendships WHERE IsAccepted = 1 AND (IdReciever = ";
    sql += std::to_string(userid);
    sql += " OR IdSender = ";
    sql += std::to_string(userid);
    sql += ");";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    dbFriendship_Id2 = dbFriendship_Id1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    db_tabelReq = 3;
    exitCode = sqlite3_exec(db, sql.c_str(), callbackCollectRequests, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    if(db_friends.size() == 0)
        return false;
    
    string ans = "";

    for(unsigned int i = 0; i < db_friends.size(); i++)
    {
        if(db_friends[i].idSender != userid)
            getName(db_friends[i].idSender, msg);
        if(db_friends[i].idReciever != userid)
            getName(db_friends[i].idReciever, msg);

        string toAdd = "";
        toAdd += "Friend: ";
        toAdd += msg;
        toAdd += "| Type: ";
        toAdd += (db_friends[i].type == 1 ? "1 (friends)\n" : "2 (close-friends)\n");

        ans += toAdd;
    }

    strcpy(msg, "");
    strcpy(msg, ans.c_str());

    return true;
}

bool getFriendReq(int userid, char *msg, int accepted)
{
    db_friends.clear();

    if(!existsId(userid, 1))
        return false;
    
    string sql = "";
    sql += "SELECT * FROM Friendships WHERE IsAccepted = ";
    sql += (accepted ? '1' : '0');
    sql += " AND IdReciever = ";
    sql += std::to_string(userid);
    sql += ";";

    DB_SQL_COMMAND

    sqlite3* db;
    int exitCode = 0;
    char *err;

    dbFriendship_Id2 = dbFriendship_Id1 = 0;

    exitCode = sqlite3_open("mydatabase.db", &db);

    if(exitCode != SQLITE_OK)
        DB_OPEN_FRIENDSHIPS_ERROR

    db_tabelReq = 3;
    exitCode = sqlite3_exec(db, sql.c_str(), callbackCollectRequests, 0, &err);

    if(exitCode != SQLITE_OK)
        DB_SELECT_FRIENDSHIPS_ERROR

    sqlite3_close(db);

    if(db_friends.size() == 0)
        return false;
    
    string ans = "";

    for(unsigned int i = 0; i < db_friends.size(); i++)
    {
        string toAdd = "";
        toAdd += "Sender: ";
        getName(db_friends[i].idSender, msg);
        toAdd += msg;
        toAdd += "| Type: ";
        toAdd += (db_friends[i].type == 1 ? "1 (friends)\n" : "2 (close-friends)\n");

        ans += toAdd;
    }

    strcpy(msg, "");
    strcpy(msg, ans.c_str());

    return true;
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
        getName(db_posts[i].postAuthor, msg);
        toAppend += msg;
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

        if(db_tabelReq == 1)
        {
            dbUser_Name = "";
            dbUser_Password = "";
            db_id = -1;
            dbUser_Admin = 0;
            dbUser_Online = 0;
            dbUser_Private = 0;
            db_roomId = 0;

            db_id = atoi(argv[0]);
            dbUser_Name += argv[1];
            dbUser_Password += argv[2];
            dbUser_Admin = atoi(argv[3]);
            dbUser_Online = atoi(argv[4]);
            dbUser_Private = atoi(argv[5]);
            db_roomId = atoi(argv[6]);
        }

        if(db_tabelReq == 2)
        {
            db_id = -1;
            dbPost_Text = "";
            dbPost_AuthorId = -1;
            dbPost_Visibility = 0;

            db_id = atoi(argv[0]);
            dbPost_AuthorId = atoi(argv[2]);
            dbPost_Text += argv[1];
            dbPost_Visibility = atoi(argv[3]);
        }

        if(db_tabelReq == 4)
        {
            db_roomId = 0;
            dbRoom_Name = "";

            db_roomId = atoi(argv[0]);
            dbRoom_Name += argv[1];
        }

        if(db_tabelReq == 5)
        {
            dbMessage_Id = 0;
            dbMessage_RoomId = 0;
            dbMessage_Text = "";
            dbMessage_UserId = 0;

            dbMessage_Id = atoi(argv[0]);
            dbMessage_RoomId = atoi(argv[1]);
            dbMessage_UserId = atoi(argv[2]);
            dbMessage_Text += argv[3];
        }
    }

    return 0;
}

int callbackGetMaxId(void *NotUsed, int argc, char **argv, char **azColName) 
{    
    NotUsed = 0;

    if(argc)
        db_callbackAns = (argv[0] ? atoi(argv[0]) : 0);

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
        if(dbFriendship_Type >= atoi(visibility))
            return true;

    if(areFriends(db_id, atoi(authorid), 1))
        if(dbFriendship_Type >= atoi(visibility))
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

    if(db_tabelReq == 3)
    {
        dbFriendship_Id1 = atoi(argv[0]);
        dbFriendship_Id2 = atoi(argv[1]);
        dbFriendship_Type = atoi(argv[2]);
        dbFriendship_Accepted = atoi(argv[3]);
    }

    return 0;
}                

int callbackCollectOnline(void *NotUsed, int argc, char **argv, char **azColName) 
{
    if(db_tabelReq == 1)
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

int callbackCollectRequests(void *NotUsed, int argc, char **argv, char **azColName)
{
    if(db_tabelReq == 3)
    {
        friendshipinfo toAdd;
        toAdd.idSender = atoi(argv[0]);
        toAdd.idReciever = atoi(argv[1]);
        toAdd.type = atoi(argv[2]);

        db_friends.push_back(toAdd);
    }

    return 0;
}