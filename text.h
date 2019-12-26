#ifndef H_TEXT
#define H_TEXT

// Mesaje intre server-client

#define T_UNKNOWN_MESSAGE \
"Mesaj necunoscut\n"

#define T_HELP \
"Comenzi:\n \
!help - Afiseaza acest mesaj.\n \
!quit - Iesire.\n \
!register - Inregistrare utilizator nou.\n \
!login - Conectare.\n \
!logout - Deconectare.\n"

#define T_QUIT \
"O zi buna! Va mai asteptam\n"

#define T_REGISTER_FAIL \
"Inregistrare esuata: Date gresite\n"

#define T_REGISTER_SUCCES \
"Inregistrare cu succes. Te poti loga!\n"

#define T_REGISTER_NAME \
"Alegeti un nume: "

#define T_REGISTER_PASS \
"Alegeti o parola: "

#define T_LOGIN_NAME \
"Nume: "

#define T_LOGIN_PASS \
"Password: "

#define T_LOGIN_FAIL \
"Login esuat. Utilizatorul nu exista sau parola este incorecta."

#define T_LOGIN_SUCCES \
"Login cu succes! Bine ati (re)venit!"

#define T_LOGOUT_FAIL \
"Logout esuat. Nu sunteti conectat."

#define T_LOGOUT_SUCCES \
"Logout cu succes."

// Mesaje eroare/informare client
#define C_WRITE_ERROR \
{\
perror("[CLIENT] Eroare la write()\n"); \
exit(1);\
}

#define C_READ_ERROR \
{\
perror("[CLIENT] Eroare la read()\n"); \
exit(1);\
}

#define C_CONNECT_ERROR \
{\
perror("[CLIENT] Eroare la connect()\n"); \
exit(1);\
}

#define C_SYNTAX_ERROR \
{\
printf("[CLIENT] Sintaxa: %s <adresa_server> <port>\n", argv[0]);\
return -1;\
}

#define C_SOCKET_ERROR \
{\
perror("[CLIENT] Eroare la socket\n");\
return -1;\
}

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

#define S_LOGGED \
printf("[SERVER] Clientul cu ID = %d s-a conectat.\n", idToSend);

#define S_LOGOUT \
printf("[SERVER] Clientul cu ID = %d s-a deconectat.\n", clientLoggedOut);

#define S_NEW_CLIENT \
printf("[SERVER] Client nou conectat\n");

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

#define DB_INSERT_USER_ERROR \
printf("[DATABASE] Eroare la inregistrarea userului\n");

#define DB_SELECT_USER_ERROR \
printf("[DATABASE] Eroare la Select() din Users\n");

#define DB_UPDATE_USER_ERROR \
printf("[DATABASE] Eroare la update in Users\n");

#define DB_CREATE_USER_OK \
printf("[DATABASE] Tabela user creata cu succes\n");

#define DB_INSERT_USER_OK \
printf("[DATABASE] Userul %s cu parola %s s-a inregistrat cu succes in tabela Users\n", nume, pass);\
printf("[DATABASE] User Id = %d\n", db_nextId);

enum EMesaje
{
    EUnknown = 0,
    EHelp,
    EQuit,
    ERegisterFail,
    ERegisterSucces,
    ELoginFail,
    ELoginSucces,
    ELogoutFail,
    ELogoutSucces
};

void ConvertToMessage(EMesaje mesaj, char *result);

#endif // H_TEXT