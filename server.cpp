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
    if((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[SERVER] Eroare la socket()\n");
        return -1;
    }

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if(bind(socketDescriptor, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[SERVER] Eroare la bind()\n");
        return -1;
    }

    if(listen(socketDescriptor, 5) == -1)
    {
        perror("[SERVER] Eroare la listen()\n");
        return -1;
    }

    if(signal(SIGCHLD, waitClosedChild) == SIG_ERR)
    {
        perror("[SERVER] Signal()\n");
        return -1;
    }

    printf("[SERVER] Asteptam la portul %d\n", PORT);

    while(1)
    {
        socklen_t length = sizeof(from);

        fflush(stdout);

        client = accept(socketDescriptor, (struct sockaddr *)&from, &length);

        if(client < 0)
        {
            perror("[SERVER] Eroare la accept\n");
            continue;
        }

        pid_t pid = fork();

        if(pid == -1)
        {
            perror("[SERVER] Eroare la fork()\n");
            return -1;
        }

        if(!pid)
        {
            printf("[SERVER] A venit un client :D\n");
            while(1)
            {
                bzero(msg, 100);
                bzero(msgrasp, 100);
                fflush(stdout);

                int commandId;
                if(read(client, &commandId, 4) <= 0)
                {
                    perror("[SERVER] Eroare la read de la client\n");
                    close(client);
                    exit(0);
                }

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
    ConvertToMessage(static_cast<EMesaje>(commandId), msg);

    if(write(client, msg, 100) < 0)
    {
        S_WRITE_ERROR;
    }

    if(commandId == 2)
    {
        close(client);
        exit(0);
    }

    return true;
}