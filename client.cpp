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
char msg[100];

int     GetCommand(char *msg);
bool    ExecuteCommand(int commandId);

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("[CLIENT] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    if((socketDescriptorToServer = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[CLIENT] Eroare la socket\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if(connect(socketDescriptorToServer, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
    {
        perror("[CLIENT] Eroare la connect().\n");
        return -1;
    }

    while(1)
    {
        bzero(msg, 100);
        fflush(stdout);

        read(0, msg, 100);

        int commandId = GetCommand(msg);
        bool ok = ExecuteCommand(commandId);
        if(!ok)
        {
            perror("[CLIENT] Eroare la execute\n");
            return -1;
        }
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

    return 0;
}

bool ExecuteCommand(int commandId)
{
    if(write(socketDescriptorToServer, &commandId, 4) <= 0)
    {
        perror("[CLIENT] Eroare la write() catre server\n");
        exit(1);
    }

    if(read(socketDescriptorToServer, msg, 100) < 0)
    {
        perror("[CLIENT] Eroare la read() de la server\n");
        exit(1);
    }

    printf("%s\n", msg);

    if(commandId == 2)
    {
        close(socketDescriptorToServer);
        exit(0);
    }

    return 1;
}