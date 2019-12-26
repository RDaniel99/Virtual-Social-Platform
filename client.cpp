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

    if(strcmp(msg + 1, "help\n") == 0)
        return 1;

    if(strcmp(msg + 1, "quit\n") == 0)
        return 2;

    if(strcmp(msg + 1, "register\n") == 0)
        return 3;

    if(strcmp(msg + 1, "login\n") == 0)
        return 4;

    if(strcmp(msg + 1, "logout\n") == 0)
        return 5;

    return 0;
}

bool ExecuteCommand(int commandId)
{
    if(write(socketDescriptorToServer, &commandId, 4) < 0)
        C_WRITE_ERROR

    if(commandId == 5 || commandId == 2)
    {
        if(write(socketDescriptorToServer, &clientId, 4) < 0)
            C_WRITE_ERROR

        clientId = -1;
    }

    if(commandId == 3 || commandId == 4)
    {
        // Registration / Login

        // Mesaj cu Name:
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
    }

    if(read(socketDescriptorToServer, msg, 1000) < 0)
        C_READ_ERROR

    printf("%s\n", msg);

    if(commandId == 2)
    {
        close(socketDescriptorToServer);
        exit(0);
    }

    if(commandId == 4)
    {
        if(read(socketDescriptorToServer, &clientId, 4) < 0)
            C_READ_ERROR
    }

    return 1;
}