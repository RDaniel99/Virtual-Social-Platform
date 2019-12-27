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
int socketDescriptorToServer;
struct sockaddr_in server;
char msg[1000];
int clientId = -1;

int     GetCommand(char *msg);
bool    ExecuteCommand(int commandId);

bool UnknownCommand();  // commandId = 0
bool HelpCommand();     // commandId = 1
bool QuitCommand();     // commandId = 2
bool RegisterCommand(); // commandId = 3
bool LoginCommand();    // commandId = 4
bool LogoutCommand();   // commandId = 5

int main(int argc, char **argv)
{
    if(argc != 3)
        C_SYNTAX_ERROR

    port = atoi(argv[2]);

    if((socketDescriptorToServer = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        C_SOCKET_ERROR

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if(connect(socketDescriptorToServer, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
        C_CONNECT_ERROR

    while(1)
    {
        bzero(msg, 1000);
        fflush(stdout);

        read(0, msg, 1000);

        int commandId = GetCommand(msg);
        bool ok = ExecuteCommand(commandId);
    }
}

int GetCommand(char *msg)
{
    if(strlen(msg) == 0)
        return 0;

    if(msg[0] != '!')
        return 0;

    if(strcmp(msg + 1, "help\n") == 0)          return 1;
    if(strcmp(msg + 1, "quit\n") == 0)          return 2;
    if(strcmp(msg + 1, "register\n") == 0)      return 3;
    if(strcmp(msg + 1, "login\n") == 0)         return 4;
    if(strcmp(msg + 1, "logout\n") == 0)        return 5;

    return 0;
}

bool ExecuteCommand(int commandId)
{
    if(write(socketDescriptorToServer, &commandId, 4) < 0)
        C_WRITE_ERROR

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
    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);
}

bool HelpCommand()
{
    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);
}

bool QuitCommand()
{
    if(write(socketDescriptorToServer, &clientId, 4) < 0)
        C_WRITE_ERROR

    clientId = -1;

    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    close(socketDescriptorToServer);
    exit(0);
}

bool RegisterCommand()
{
    if(write(socketDescriptorToServer, &clientId, 4) <= 0)  C_WRITE_ERROR

    int testIdResult = -1;

    if(read(socketDescriptorToServer, &testIdResult, 4) <= 0) C_READ_ERROR
    if(testIdResult == 0)
    {
        if(read(socketDescriptorToServer, msg, 1000) < 0)   C_READ_ERROR
        if(write(0, msg, 1000) <= 0)                        C_WRITE_ERROR

        return 0;
    }

    // Mesaj cu nume
    bzero(msg, 1000);
    if(read(socketDescriptorToServer, msg, 1000) <= 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)                          C_WRITE_ERROR

    fflush(stdout);
    // Transmitere nume
    bzero(msg, 1000);
    if(read(0, msg, 1000) <= 0)                          C_READ_ERROR
    if(write(socketDescriptorToServer, msg, 1000) < 0)   C_WRITE_ERROR

    // Mesaj cu Pass
    bzero(msg, 1000);
    if(read(socketDescriptorToServer, msg, 1000) <= 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)                          C_WRITE_ERROR

    fflush(stdout);
    // Transmitere Pass
    bzero(msg, 1000);
    if(read(0, msg, 1000) <= 0)                          C_READ_ERROR
    if(write(socketDescriptorToServer, msg, 1000) < 0)   C_WRITE_ERROR

    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);
}

bool LoginCommand()
{
    if(write(socketDescriptorToServer, &clientId, 4) <= 0)  C_WRITE_ERROR

    int testIdResult = -1;

    if(read(socketDescriptorToServer, &testIdResult, 4) <= 0) C_READ_ERROR
    if(testIdResult == 0)
    {
        if(read(socketDescriptorToServer, msg, 1000) < 0)   C_READ_ERROR
        if(write(0, msg, 1000) <= 0)                        C_WRITE_ERROR

        return 0;
    }

    // Mesaj cu nume
    bzero(msg, 1000);
    if(read(socketDescriptorToServer, msg, 1000) <= 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)                          C_WRITE_ERROR

    fflush(stdout);
    // Transmitere nume
    bzero(msg, 1000);
    if(read(0, msg, 1000) <= 0)                          C_READ_ERROR
    if(write(socketDescriptorToServer, msg, 1000) < 0)   C_WRITE_ERROR

    // Mesaj cu Pass
    bzero(msg, 1000);
    if(read(socketDescriptorToServer, msg, 1000) <= 0)   C_READ_ERROR
    if(write(0, msg, 1000) < 0)                          C_WRITE_ERROR

    fflush(stdout);
    // Transmitere Pass
    bzero(msg, 1000);
    if(read(0, msg, 1000) <= 0)                          C_READ_ERROR
    if(write(socketDescriptorToServer, msg, 1000) < 0)   C_WRITE_ERROR

    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    if(read(socketDescriptorToServer, &clientId, 4) < 0)
        C_READ_ERROR
} 

bool LogoutCommand()
{
    if(write(socketDescriptorToServer, &clientId, 4) < 0)
        C_WRITE_ERROR

    clientId = -1;

    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);
}