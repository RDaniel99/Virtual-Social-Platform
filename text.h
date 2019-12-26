#ifndef H_TEXT
#define H_TEXT

// Mesaje intre server-client

#define T_UNKNOWN_MESSAGE \
"Mesaj necunoscut\n"

#define T_HELP \
"Comenzi:\n \
!help - Afiseaza acest mesaj.\n \
!quit - Iesire.\n"

#define T_QUIT \
"O zi buna! Va mai asteptam\n"

// Mesaje eroare/informare server

#define S_WRITE_ERROR \
{\
perror("[SERVER] Eroare la write()\n"); \
exit(1);\
}

#define S_SOCKET_ERROR \
{\
perror("[SERVER] Eroare la socket()\n");\
exit(1);\
}

#define S_BIND_ERROR \
{\
perror("[SERVER] Eroare la bind()\n");\
exit(1);\
}

#define S_LISTEN_ERROR \
{\
perror("[SERVER] Eroare la listen()\n");\
exit(1);\
}

#define S_SIGNAL_ERROR \
{\
perror("[SERVER] Signal()\n");\
exit(1);\
}

#define S_ACCEPT_ERROR \
{\
perror("[SERVER] Eroare la accept()\n");\
exit(1);\
}

#define S_FORK_ERROR \
{\
perror("Eroare la fork()\n");\
exit(1);\
}

#define S_READ_ERROR \
{\
perror("[SERVER] Eroare la read de la client\n");\
close(client);\
exit(0);\
}

#define S_DB_CREATE_ERROR \
{\
perror("[SERVER] Eroare la createDatabases()\n");\
exit(0);\
}

#define S_NEW_CLIENT \
printf("Client nou conectat\n");

// Mesaje de eroare/informare DB

#define DB_OPEN_USER_ERROR \
{\
printf("[DATABASE] Eroare la open() - tabela Users");\
return false;\
}

#define DB_CREATE_USER_ERROR \
{\
printf("[DATABASE] Eroare la create table - tabela Users\n %s\n", err);\
sqlite3_free(err);\
sqlite3_close(db);\
return false;\
}

#define DB_CREATE_USER_OK \
printf("[DATABASE] Tabela user creata cu succes\n");

enum EMesaje
{
    EUnknown = 0,
    EHelp,
    EQuit,
};

void ConvertToMessage(EMesaje mesaj, char *result);

#endif // H_TEXT