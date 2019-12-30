#ifndef H_DATABASE
#define H_DATABASE

bool    createDatabasePosts();
bool    createDatabaseUsers();
bool    createDatabaseFriendships();
bool    createDatabases();

bool    validateName(char *nume);
bool    validatePass(char *pass);
bool    validateAdmin(int isAdmin);
bool    validatePrivacy(int privacy);
bool    validatePostText(char *postText);
bool    validateOwnerId(int ownerid);
bool    validateVisibility(int visibility);

bool    addFriend(int senderId, int recieverId, int type);
bool    addPost(char *postText, int ownerid, int visibility);
bool    addUser(char *nume, char* pass, int isAdmin = 0, int privacy = 0);

bool    deleteFriend(int userid, int friendid);
bool    deletePost(int postid, int userid);

bool    loginUser(char *nume, char* pass);
bool    updateOn(int userid, int value);

bool    updateUser(char *nume, char *pass, int privacy, int userid);
bool    updatePost(int postid, char *postText, int ownerid, int visibility);

bool    getOnline(char *msg);
bool    getPosts(int userid, char *msg, bool areAll = false);
bool    getFriendReq(int userid, char *msg, int accepted = 0);
bool    getFriends(int userid, char *msg);
bool    getName(int userid, char *msg);

bool    existsId(int id, int tabel);
bool    existsName(char *nume);
bool    isPostValid(char *authorid, char *visibility);
bool    isUserAdmin(int userid);
bool    areFriends(int id1, int id2, int accepted);

int     computeNextId(int tabel);
int     getIdFromLastSelect();

int     callbackCollectRequests(void *, int, char **, char **);
int     callbackCheckIfExists(void *, int, char **, char **);
int     callbackGetMaxId(void *, int, char **, char **);
int     callbackCollectPosts(void *, int, char **, char **);
int     callbackAreInFriends(void *, int, char **, char **);
int     callbackCollectAllPosts(void *, int, char **, char **);
int     callbackCollectOnline(void *, int, char **, char **);

#endif //H_DATABASE