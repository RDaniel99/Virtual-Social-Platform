#define H_CLIENT

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include "text.h"

using namespace std;

int port;
int sd;
struct sockaddr_in server;
char msg[1000];
int clientId = -1;

int     GetCommand(char *msg);
bool    ExecuteCommand(int commandId);
bool    PassTestId();

bool    UnknownCommand();               // commandId = 0
bool    HelpCommand();                  // commandId = 1
bool    QuitCommand();                  // commandId = 2
bool    RegisterCommand(int isAdmin);   // commandId = 3 sau 7
bool    LoginCommand();                 // commandId = 4
bool    LogoutCommand();                // commandId = 5
bool    ShowPostsCommand();             // commandId = 6
bool    AddPostCommand();               // commandId = 8
bool    DeletePostCommand();            // commandId = 9
bool    OnlineCommand();                // commandId = 10
bool    EditPostCommand();              // commandId = 11
bool    EditProfileCommand();           // commandId = 12
bool    AddFriendCommand();             // commandId = 13
bool    RequestsCommand();              // commandId = 14
bool    FriendsCommand();               // commandId = 15
bool    RemoveFriendCommand();          // commandId = 16
bool    AcceptRequestCommand();         // commandId = 17
bool    RemoveRequestCommand();         // commandId = 18

int main(int argc, char **argv)
{
    if(argc != 3)
        C_SYNTAX_ERROR

    port = atoi(argv[2]);

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        C_SOCKET_ERROR

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if(connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
        C_CONNECT_ERROR

    C_CONNECT_SUCCES

    while(1)
    {
        bzero(msg, 1000);
        fflush(stdout);

        read(0, msg, 1000);

        int commandId = GetCommand(msg);
        bool ok = ExecuteCommand(commandId);
    }
}

bool PassTestId()
{
    if(write(sd, &clientId, 4) < 0)
        C_WRITE_ERROR

    int testId = -1;

    if(read(sd, &testId, 4) < 0)
        C_READ_ERROR

    if(!testId)
    {
        if(read(sd, msg, 1000) < 0)
            C_READ_ERROR

        printf("%s\n", msg);

        return false;
    }

    return true;
}

int GetCommand(char *msg)
{
    if(strlen(msg) == 0)
        return ECUnknown;

    if(msg[0] != '!')
        return ECUnknown;

    if(strcmp(msg + 1, "help\n") == 0)              return ECHelp;
    if(strcmp(msg + 1, "quit\n") == 0)              return ECQuit;
    if(strcmp(msg + 1, "register\n") == 0)          return ECRegister;
    if(strcmp(msg + 1, "login\n") == 0)             return ECLogin;
    if(strcmp(msg + 1, "logout\n") == 0)            return ECLogout;
    if(strcmp(msg + 1, "showposts\n") == 0)         return ECShowPosts;
    if(strcmp(msg + 1, "registera\n") == 0)         return ECRegisterA;
    if(strcmp(msg + 1, "addpost\n") == 0)           return ECAddPost;
    if(strcmp(msg + 1, "deletepost\n") == 0)        return ECDeletePost;
    if(strcmp(msg + 1, "online\n") == 0)            return ECOnline;
    if(strcmp(msg + 1, "editpost\n") == 0)          return ECEditPost;
    if(strcmp(msg + 1, "editprofile\n") == 0)       return ECEditProfile;
    if(strcmp(msg + 1, "addfriend\n") == 0)         return ECAddFriend;
    if(strcmp(msg + 1, "requests\n") == 0)          return ECRequests;
    if(strcmp(msg + 1, "friends\n") == 0)           return ECShowFriends;
    if(strcmp(msg + 1, "removefriend\n") == 0)      return ECRemoveFriend;
    if(strcmp(msg + 1, "acceptrequest\n") == 0)     return ECAcceptRequest;
    if(strcmp(msg + 1, "removerequest\n") == 0)     return ECRemoveRequest;

    return ECUnknown;
}

bool ExecuteCommand(int commandId)
{
    if(write(sd, &commandId, 4) < 0)
        C_WRITE_ERROR

    switch(commandId)
    {
        case ECUnknown:         return UnknownCommand();
        case ECHelp:            return HelpCommand();
        case ECQuit:            return QuitCommand();
        case ECRegister:        return RegisterCommand(0);
        case ECLogin:           return LoginCommand();
        case ECLogout:          return LogoutCommand();
        case ECShowPosts:       return ShowPostsCommand();
        case ECRegisterA:       return RegisterCommand(1);
        case ECAddPost:         return AddPostCommand();
        case ECDeletePost:      return DeletePostCommand();
        case ECOnline:          return OnlineCommand();
        case ECEditPost:        return EditPostCommand();
        case ECEditProfile:     return EditProfileCommand();
        case ECAddFriend:       return AddFriendCommand();
        case ECRequests:        return RequestsCommand();
        case ECShowFriends:     return FriendsCommand();
        case ECRemoveFriend:    return RemoveFriendCommand();
        case ECAcceptRequest:   return AcceptRequestCommand();
        case ECRemoveRequest:   return RemoveRequestCommand();
    }

    return false;
}

bool UnknownCommand()
{
    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    return 0;
}

bool HelpCommand()
{
    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    return true;
}

bool QuitCommand()
{
    if(write(sd, &clientId, 4) < 0)
        C_WRITE_ERROR

    clientId = -1;

    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    close(sd);
    exit(0);
}

bool RegisterCommand(int isAdmin)
{
    if(!PassTestId())
        return true;

    // Mesaj cu tipul de register
    if(write(sd, &isAdmin, 4) < 0)      C_WRITE_ERROR

    // Mesaj cu nume
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)         C_READ_ERROR
    if(write(0, msg, 1000) < 0)         C_WRITE_ERROR

    fflush(stdout);
    // Transmitere nume
    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)          C_READ_ERROR
    if(write(sd, msg, 1000) < 0)        C_WRITE_ERROR

    // Mesaj cu Pass
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)         C_READ_ERROR
    if(write(0, msg, 1000) < 0)         C_WRITE_ERROR

    fflush(stdout);
    // Transmitere Pass
    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)          C_READ_ERROR
    if(write(sd, msg, 1000) < 0)        C_WRITE_ERROR

    // Mesaj cu Privacy
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)         C_READ_ERROR
    if(write(0, msg, 1000) < 0)         C_WRITE_ERROR

    // Transmitere Privacy
    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)          C_READ_ERROR
    if(write(sd, msg, 1000) < 0)        C_WRITE_ERROR

    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    return true;
}

bool LoginCommand()
{
    if(!PassTestId())
        return true;

    // Mesaj cu nume
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    fflush(stdout);
    // Transmitere nume
    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Mesaj cu Pass
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    fflush(stdout);
    // Transmitere Pass
    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    if(read(sd, &clientId, 4) < 0)
        C_READ_ERROR

    return true;
} 

bool LogoutCommand()
{
    if(write(sd, &clientId, 4) < 0)
        C_WRITE_ERROR

    clientId = -1;

    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    return true;
}

bool ShowPostsCommand()
{
    if(write(sd, &clientId, 4) < 0)
        C_WRITE_ERROR

    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    return true;
}

bool AddPostCommand()
{
    if(!PassTestId())
        return true;

    // post text
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)    C_READ_ERROR
    if(write(sd, msg, 1000) < 0)  C_WRITE_ERROR

    // post type
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)    C_READ_ERROR
    if(write(sd, msg, 1000) < 0)  C_WRITE_ERROR

    // result
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    return true;
}

bool DeletePostCommand()
{
    if(!PassTestId())
        return true;

    // post id
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)                         C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)                          C_READ_ERROR
    if(write(sd, msg, 1000) < 0)  C_WRITE_ERROR

    // result
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)                         C_WRITE_ERROR

    return true;
}

bool OnlineCommand()
{
    if(!PassTestId())
        return true;

    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    return true;
}

bool EditPostCommand()
{
    if(!PassTestId())
        return true;

    // Post Id
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)    C_READ_ERROR
    if(write(sd, msg, 1000) < 0)  C_WRITE_ERROR

    // Post text
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)    C_READ_ERROR
    if(write(sd, msg, 1000) < 0)  C_WRITE_ERROR

    // Post Type
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)    C_READ_ERROR
    if(write(sd, msg, 1000) < 0)  C_WRITE_ERROR

    // Result
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)   C_WRITE_ERROR

    return true;
}

bool EditProfileCommand()
{
    if(!PassTestId())
        return true;

    // Nume
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Parola
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Tip de cont
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Rezultat
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    return true;
}

bool AddFriendCommand()
{
    if(write(sd, &clientId, 4) < 0)
        C_WRITE_ERROR

    int testId = -1;

    if(read(sd, &testId, 4) < 0)
        C_READ_ERROR

    if(!testId)
    {
        if(read(sd, msg, 1000) < 0)
            C_READ_ERROR

        printf("%s\n", msg);

        return true;
    }

    // ID Friend
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Tip Friend
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Result
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    return true;
}

bool RequestsCommand()
{
    if(!PassTestId())
        return true;

    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR
    
    if(write(0, msg, 1000) < 0)
        C_WRITE_ERROR
    
    return true;
}

bool FriendsCommand()
{
    if(!PassTestId())
        return true;

    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)
        C_READ_ERROR
    
    if(write(0, msg, 1000) < 0)
        C_WRITE_ERROR
    
    return true;
}

bool RemoveFriendCommand()
{
    if(!PassTestId())
        return true;
    
    // ID friend
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Raspuns
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    return true;
}

bool AcceptRequestCommand()
{
    if(!PassTestId())
        return true;
    
    // ID friend
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Raspuns
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    return true;
}

bool RemoveRequestCommand()
{
    if(!PassTestId())
        return true;
    
    // ID friend
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    bzero(msg, 1000);
    if(read(0, msg, 1000) < 0)      C_READ_ERROR
    if(write(sd, msg, 1000) < 0)    C_WRITE_ERROR

    // Raspuns
    fflush(stdout);
    bzero(msg, 1000);
    if(read(sd, msg, 1000) < 0)     C_READ_ERROR
    if(write(0, msg, 1000) < 0)     C_WRITE_ERROR

    return true;
}