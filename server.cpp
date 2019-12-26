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
                bzero(msg, 1000);
                bzero(msgrasp, 1000);
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

    if(commandId == 5)
    {
        int clientLoggedOut = -1;

        if(read(client, &clientLoggedOut, 4) <= 0)
            S_READ_ERROR
        
        if(updateOn(clientLoggedOut, 0))
            msgId = 8;
        else
            msgId = 7;
        
        if(clientLoggedOut > -1)
            S_LOGOUT
    }

    if(commandId == 3 || commandId == 4)
    {
        printf("[SERVER] Ok se cere nume\n");
        // Registration command / Login command
        strcpy(msg, "");
        strcpy(msg, (commandId == 3 ? T_REGISTER_NAME : T_LOGIN_NAME));

        if(write(client, msg, 1000) < 0)
            S_WRITE_ERROR

        char nume[1000];
        if(read(client, nume, 1000) <= 0)
            S_READ_ERROR

        strcpy(msg, "");
        strcpy(msg, (commandId == 3 ? T_REGISTER_PASS : T_LOGIN_PASS));

        printf("[SERVER] Ok se cere pass\n");
        if(write(client, msg, 1000) < 0)
            S_WRITE_ERROR

        char pass[1000];
        if(read(client, pass, 1000) <= 0)
            S_READ_ERROR

        // Se elimina \n de la final
        nume[strlen(nume) - 1] = 0;
        pass[strlen(pass) - 1] = 0;

        if(commandId == 3)
            printf("[SERVER] Ok se inregistreaza...\n\n");
        else
            printf("[SERVER] Ok se logheaza...\n\n");
        
        if(commandId == 3)
        {
            if(insertUser(nume, pass))
                msgId = 4;
            else
                msgId = 3;
        }
        else
        {
            if(loginUser(nume, pass))
                msgId = 6;
            else
                msgId = 5;
        } 
    }

    ConvertToMessage(static_cast<EMesaje>(msgId), msg);

    if(write(client, msg, 1000) < 0)
        S_WRITE_ERROR

    if(commandId == 2)
    {
        int clientLoggedOut = -1;

        if(read(client, &clientLoggedOut, 4) <= 0)
            S_READ_ERROR
        
        updateOn(clientLoggedOut, 0);

        if(clientLoggedOut > -1)
            S_LOGOUT

        close(client);
        exit(0);
    }

    if(commandId == 4)
    {
        int idToSend = (msgId == 6 ? getIdFromLastSelect() : -1);

        if(idToSend > -1)
            S_LOGGED
        
        if(write(client, &idToSend, 4) < 0)
            S_WRITE_ERROR
        
        if(msgId == 6)
            updateOn(idToSend, 1);
    }

    return true;
}