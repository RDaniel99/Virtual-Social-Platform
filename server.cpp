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
char msg[100];
char msgrasp[100] = " ";
int socketDescriptor;
int client;

void waitClosedChild(int sig);
bool ExecuteCommand(int commandId, char* msg);

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
                bzero(msg, 100);
                bzero(msgrasp, 100);
                fflush(stdout);

                int commandId;
                if(read(client, &commandId, 4) <= 0)
                    S_READ_ERROR

                printf("[SERVER] Mesajul a fost receptionat: %d\n", commandId);

                bool ok = ExecuteCommand(commandId, msgrasp);
            }
        }
    }

    return 0;
}

void waitClosedChild(int sig)
{
    wait(NULL);
}

bool ExecuteCommand(int commandId, char *msg)
{
    int msgId = commandId;

    if(commandId == 3)
    {
        printf("[SERVER] Ok se cere nume\n");
        // Registration command
        strcpy(msg, "");
        strcpy(msg, T_REGISTER_NAME);

        if(write(client, msg, 100) < 0)
            S_WRITE_ERROR

        char nume[100];
        if(read(client, nume, 100) <= 0)
            S_READ_ERROR

        strcpy(msg, "");
        strcpy(msg, T_REGISTER_PASS);

        printf("[SERVER] Ok se cere pass\n");
        if(write(client, msg, 100) < 0)
            S_WRITE_ERROR

        char pass[100];
        if(read(client, pass, 100) <= 0)
            S_READ_ERROR

        // Se elimina \n de la final
        nume[strlen(nume) - 1] = 0;
        pass[strlen(pass) - 1] = 0;

        printf("[SERVER] Ok se inregistreaza...\n\n");
        if(insertUser(nume, pass))
            msgId = 4;
    }

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 100) < 0)
        S_WRITE_ERROR

    if(commandId == 2)
    {
        close(client);
        exit(0);
    }

    return true;
}