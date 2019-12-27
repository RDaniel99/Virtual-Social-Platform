#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include "database.h"
#include "text.h"

using namespace std;

#define PORT 1307

struct sockaddr_in  server;
struct sockaddr_in  from;
char msg[1000];
char msgrasp[1000] = " ";
int socketDescriptor;
int client;

void waitClosedChild(int sig);
bool ExecuteCommand(int commandId);

bool UnknownCommand();  // commandId = 0
bool HelpCommand();     // commandId = 1
bool QuitCommand();     // commandId = 2
bool RegisterCommand(); // commandId = 3
bool LoginCommand();    // commandId = 4
bool LogoutCommand();   // commandId = 5

int main()
{
    if(!createDatabases())
        S_DB_CREATE_ERROR

    if((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        S_SOCKET_ERROR

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if(bind(socketDescriptor, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
        S_BIND_ERROR

    if(listen(socketDescriptor, 5) == -1)
        S_LISTEN_ERROR

    if(signal(SIGCHLD, waitClosedChild) == SIG_ERR)
        S_SIGNAL_ERROR

    printf("[SERVER] Asteptam la portul %d\n", PORT);

    while(1)
    {
        socklen_t length = sizeof(from);

        fflush(stdout);

        client = accept(socketDescriptor, (struct sockaddr *)&from, &length);

        if(client < 0)
            S_ACCEPT_ERROR

        pid_t pid = fork();

        if(pid == -1)
            S_FORK_ERROR

        if(!pid)
        {
            S_NEW_CLIENT
            while(1)
            {
                bzero(msg, 1000);
                bzero(msgrasp, 1000);
                fflush(stdout);

                int commandId;
                if(read(client, &commandId, 4) <= 0)
                    S_READ_ERROR

                printf("[SERVER] Mesajul a fost receptionat: %d\n", commandId);

                bool ok = ExecuteCommand(commandId);
            }
        }
    }

    return 0;
}

void waitClosedChild(int sig)
{
    wait(NULL);
}

bool ExecuteCommand(int commandId)
{
    switch(commandId)
    {
        case ECUnknown:     return UnknownCommand();
        case ECHelp:        return HelpCommand();
        case ECQuit:        return QuitCommand();
        case ECRegister:    return RegisterCommand();
        case ECLogin:       return LoginCommand();
        case ECLogout:      return LogoutCommand();
    }

    return false;
}

bool UnknownCommand()
{
    int msgId = 0;

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    return true;
}

bool HelpCommand()
{
    int msgId = 1;

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    return true;
}

bool QuitCommand()
{
    int msgId = 2;

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    int clientLoggedOut = -1;

    if(read(client, &clientLoggedOut, 4) <= 0)
        S_READ_ERROR
    
    updateOn(clientLoggedOut, 0);

    if(clientLoggedOut > -1)
        S_LOGOUT

    close(client);
    exit(0);
}

bool RegisterCommand()
{
    int msgId = -1;
    int tempId = -1;

    if(read(client, &tempId, 4) <= 0)
        S_READ_ERROR

    int testIdResult = 0;
    if(tempId == -1)
        testIdResult = 1;
    
    if(write(client, &testIdResult, 4) < 0)
        S_WRITE_ERROR

    if(testIdResult == 0)
    {
        strcpy(msg, "");
        strcpy(msg, T_USER_ALREADY_LOGGED);

        if(write(client, msg, 1000) < 0)
            S_WRITE_ERROR

        return true;
    }
    
    strcpy(msg, "");
    strcpy(msg, T_REGISTER_NAME);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    char nume[1000];
    if(read(client, nume, 1000) <= 0)
        S_READ_ERROR

    strcpy(msg, "");
    strcpy(msg, T_REGISTER_PASS);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    char pass[1000];
    if(read(client, pass, 1000) <= 0)
        S_READ_ERROR

    // Se elimina \n de la final
    nume[strlen(nume) - 1] = 0;
    pass[strlen(pass) - 1] = 0;
        
    if(insertUser(nume, pass))
        msgId = 4;
    else
        msgId = 3;

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR
}

bool LoginCommand()
{
    int msgId = -1;
    int tempId = -1;

    if(read(client, &tempId, 4) <= 0)
        S_READ_ERROR

    int testIdResult = 0;
    if(tempId == -1)
        testIdResult = 1;
    
    if(write(client, &testIdResult, 4) < 0)
        S_WRITE_ERROR

    if(testIdResult == 0)
    {
        strcpy(msg, "");
        strcpy(msg, T_USER_ALREADY_LOGGED);

        if(write(client, msg, 1000) < 0)
            S_WRITE_ERROR

        return true;
    }
    
    strcpy(msg, "");
    strcpy(msg, T_LOGIN_NAME);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    char nume[1000];
    if(read(client, nume, 1000) <= 0)
        S_READ_ERROR

    strcpy(msg, "");
    strcpy(msg, T_LOGIN_PASS);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    char pass[1000];
    if(read(client, pass, 1000) <= 0)
        S_READ_ERROR

    // Se elimina \n de la final
    nume[strlen(nume) - 1] = 0;
    pass[strlen(pass) - 1] = 0;
        
    if(loginUser(nume, pass))
        msgId = 6;
    else
        msgId = 5;

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    int idToSend = (msgId == 6 ? getIdFromLastSelect() : -1);

    if(idToSend > -1)
        S_LOGGED
        
    if(write(client, &idToSend, 4) < 0)
        S_WRITE_ERROR
        
    if(msgId == 6)
        updateOn(idToSend, 1);

    return true;
}

bool LogoutCommand()
{
    int msgId = -1;

    int clientLoggedOut = -1;

    if(read(client, &clientLoggedOut, 4) <= 0)
        S_READ_ERROR
        
    if(updateOn(clientLoggedOut, 0))
        msgId = 8;
    else
        msgId = 7;
        
    if(clientLoggedOut > -1)
        S_LOGOUT

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR
}