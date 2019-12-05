
#include "utils.h"

extern int errno;

int sayHello(int fd);

int main ()
{
    struct sockaddr_in  server;
    struct sockaddr_in  from;
    struct timeval      tv;

    fd_set readfds;
    fd_set actfds;

    int sd, client, optval = 1, fd, nfds, len;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server] Eroare la socket().\n");
        return errno;
    }

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));


    bzero (&server, sizeof (server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server] Eroare la bind().\n");
        return errno;
    }

    if (listen (sd, 5) == -1)
    {
        perror ("[server] Eroare la listen().\n");
        return errno;
    }

    FD_ZERO (&actfds);
    FD_SET (sd, &actfds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    nfds = sd;

    printf ("[server] Asteptam la portul %d...\n", PORT);
    fflush (stdout);

    while (1)
    {
        bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

        /* apelul select() */
        if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
        {
            perror ("[server] Eroare la select().\n");
            return errno;
        }
        if (FD_ISSET (sd, &readfds))
        {
            len = sizeof (from);
            bzero (&from, sizeof (from));
            client = -1;

            //client = accept (sd, (struct sockaddr *) &from, &len);

            if (client < 0)
            {
                perror ("[server] Eroare la accept().\n");
                continue;
            }

            if (nfds < client) /* ajusteaza valoarea maximului */
                nfds = client;

            FD_SET (client, &actfds);

            printf("[server] S-a conectat clientul cu descriptorul %d.\n",client);
            fflush (stdout);
        }
        for (fd = 0; fd <= nfds; fd++)
        {
            if (fd != sd && FD_ISSET (fd, &readfds))
            {
                if (sayHello(fd))
                {
                    printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
                    fflush (stdout);
                    close (fd);
                    FD_CLR (fd, &actfds);

                }
            }
        }
    }
}

int sayHello(int fd)
{
    char buffer[100];
    int bytes;
    char msg[100];
    char msgrasp[100]=" ";

    bytes = read (fd, msg, sizeof (buffer));
    if (bytes < 0)
    {
        perror ("Eroare la read() de la client.\n");
        return 0;
    }
    printf ("[server]Mesajul a fost receptionat...%s\n", msg);

    bzero(msgrasp,100);
    strcat(msgrasp,"Hello ");
    strcat(msgrasp,msg);

    printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);

    if (bytes && write (fd, msgrasp, bytes) < 0)
    {
        perror ("[server] Eroare la write() catre client.\n");
        return 0;
    }

    return bytes;
}