#ifndef H_DATABASE
#define H_DATABASE

#include <set>

bool createDatabasePosts();
bool createDatabaseUsers();
bool createDatabaseFriendships();
bool createDatabases();

bool validateName(char *nume);
bool validatePass(char *pass);
bool validateAdmin(int isAdmin);
bool insertUser(char *nume, char* pass, int isAdmin = 0);
bool loginUser(char *nume, char* pass);
bool updateOn(int userid, int value);

bool editPost(int postid, char *postText, int ownerid, int visibility);
bool getOnline(char *msg);
bool getPosts(int userid, char *msg, bool areAll = false);
bool existsId(int id, int tabel);
int computeNextId(int tabel);
int getIdFromLastSelect();
bool existsName(char *nume);
bool isPostValid(char *authorid, char *visibility);
bool isUserAdmin(int userid);
bool areFriends(int id1, int id2, int accepted);
bool validatePostText(char *postText);
bool validateOwnerId(int ownerid);
bool validateVisibility(int visibility);
bool addPost(char *postText, int ownerid, int visibility);
bool deletePost(int postid, int userid);

int callbackCheckIfExists(void *, int, char **, char **);
int callbackGetMaxId(void *, int, char **, char **);
int callbackCollectPosts(void *, int, char **, char **);
int callbackAreInFriends(void *, int, char **, char **);
int callbackCollectAllPosts(void *, int, char **, char **);
int callbackCollectOnline(void *, int, char **, char **);

#endif //H_DATABASE