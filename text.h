#ifndef H_TEXT
#define H_TEXT

#define BUFF_SIZE 1500

enum EMesaje
{
    EMUnknown = 0,
    EMHelp,
    EMQuit,
    EMRegisterFail,
    EMRegisterSucces,
    EMLoginFail,
    EMLoginSucces,
    EMLogoutFail,
    EMLogoutSucces,
    EMAddPostFail,
    EMAddPostSucces,
    EMDeletePostFail,
    EMDeletePostSucces,
    EMEditPostFail,
    EMEditPostSucces,
    EMEditProfileFail,
    EMEditProfileSucces,
    EMSentFriendRequestFail,
    EMSentFriendRequestSucces,
    EMRemoveFriendFail,
    EMRemoveFriendSucces,
    EMAcceptRequestFail,
    EMAcceptRequestSucces,
    EMRemoveRequestFail,
    EMRemoveRequestSucces,
    EMJoinRoomFail,
    EMJoinRoomSucces,
    EMCreateRoomFail,
    EMCreateRoomSucces,
    EMMessageFail,
    EMLeaveRoomFail,
    EMLeaveRoomSucces,
    EMDeleteRoomFail,
    EMDeleteRoomSucces,
    EMRefreshFail
};

enum EComenzi
{
    ECUnknown = 0,
    ECHelp,
    ECQuit,
    ECRegister,
    ECLogin,
    ECLogout,
    ECShowPosts,
    ECRegisterA,
    ECAddPost,
    ECDeletePost,
    ECOnline,
    ECEditPost,
    ECEditProfile,
    ECAddFriend,
    ECRequests,
    ECShowFriends,
    ECRemoveFriend,
    ECAcceptRequest,
    ECRemoveRequest,
    ECShowRooms,
    ECJoinRoom,
    ECCreateRoom,
    ECMessage,
    ECLeaveRoom,
    ECRefresh,
    ECDeleteRoom,
    ECRoomMembers
};

void ConvertToMessage(EMesaje mesaj, char *result);


// Mesaje intre server-client

#ifdef H_SERVER

#define T_UNKNOWN_MESSAGE \
"Mesaj necunoscut\n"

#define T_HELP \
"Comenzi social media:\n \
!help           - Afiseaza acest mesaj.\n \
!quit           - Iesire.\n \
!register       - Inregistrare utilizator nou.\n \
!login          - Conectare.\n \
!logout         - Deconectare.\n \
!showposts      - Afiseaza postarile din newsfeed.\n \
!addpost        - Adauga o postare.\n \
!deletepost     - Sterge o postare.\n \
!online         - Afiseaza utilizatorii logati.\n \
!editpost       - Editeaza o postare.\n \
!editprofile    - Editeaza propriul profil.\n \
!addfriend      - Adauga un prieten.\n \
!requests       - Afiseaza cererile de prietenie primite.\n \
!friends        - Afiseaza prietenii.\n \
!removefriend   - Sterge un prieten.\n \
!acceptrequest  - Accepta o cerere de prietenie.\n \
!removerequest  - Sterge o cerere de prietenie.\n\n\
Comenzi chat rooms:\n \
!showrooms      - Afiseaza camerele de chat.\n \
!joinroom       - Intra intr-o camera de chat.\n \
!createroom     - Porneste o camera de chat.\n \
!message        - Trimite un mesaj catre ceilalti din camera.\n \
!leaveroom      -.Paraseste camera de chat.\n \
!refresh        - Refresh la chat.\n \
!deleteroom     - Sterge o camera de chat.\n \
!members        - Afiseaza participantii din chat.\n\n"

#define T_QUIT \
"O zi buna! Va mai asteptam\n"

#define T_MESSAGE \
"Mesaj: "

#define T_REFRESH_FAIL \
"Niciun mesaj nou sau nu sunteti intr-un room.\n"

#define T_USER_ALREADY_LOGGED \
"Sunteti deja conectat!\n"

#define T_NEED_LOGIN \
"Trebuie sa va logati ca sa folositi aceasta comanda!\n"

#define T_NO_USER_ONLINE \
"Niciun utilizator conectat!\n"

#define T_NO_POST_TO_SHOW \
"Nicio postare de afisat.\n"

#define T_NO_FRIEND_REQ \
"Nicio cerere de prietenie fara raspuns.\n"

#define T_NO_FRIEND_CONNECTED \
"Niciun prieten.\n"

#define T_REGISTER_FAIL \
"Inregistrare esuata: Date gresite\n"

#define T_REGISTER_SUCCES \
"Inregistrare cu succes. Te poti loga!\n"

#define T_REGISTER_NAME \
"Alegeti un nume: "

#define T_REGISTER_PASS \
"Alegeti o parola (minim 4 caractere): "

#define T_REGISTER_PRIVACY \
"Tip de cont (0 - public, 1 - privat): "

#define T_REMOVE_FRIEND_FAIL \
"Utilizatorul nu a fost sters din lista de prieteni. Utilizatorul nu exista / nu va e prieten.\n"

#define T_REMOVE_FRIEND_SUCCES \
"Utilizatorul a fost sters cu succes din lista de prieteni.\n"

#define T_ACCEPT_REQUEST_FAIL \
"Cererea de prietenie nu a fost acceptata. ID Friend Request invalid.\n"

#define T_ACCEPT_REQUEST_SUCCES \
"Cererea de pretenie a fost acceptata cu succes. Sunteti acum prieteni!\n"

#define T_REMOVE_REQUEST_FAIL \
"Cererea de prietenie nu a fost stearsa. ID Friend Request invalid.\n"

#define T_REMOVE_REQUEST_SUCCES \
"Cererea de prietenie a fost stearsa cu succes.\n"

#define T_JOIN_ROOM_FAIL \
"Nu v-ati putut alatura camerei de chat. Camera inexistenta sau sunteti deja intr-o alta camera.\n"

#define T_JOIN_ROOM_SUCCES \
"V-ati alaturat camerei de chat cu succes!\n"

#define T_CREATE_ROOM_FAIL \
"Nu ati putut sa incepeti o camera noua de chat. Exista deja una cu acest nume sau sunteti deja intr-o camera.\n"

#define T_CREATE_ROOM_SUCCES \
"Ati creat o camera de chat cu succes.\n"

#define T_MESSAGE_FAIL \
"Nu s-a putut transmite mesajul. Nu sunteti intr-o camera de chat.\n"

#define T_LEAVE_ROOM_FAIL \
"Trebuie sa fiti intr-o camera de chat ca sa puteti iesi.\n"

#define T_LEAVE_ROOM_SUCCES \
"Ati parasit camera de chat cu succes.\n"

#define T_DELETE_ROOM_FAIL \
"Camera de chat nu a putut fi stearsa. Nu aveti drepturi / camera nu exista / camera are in continuare membrii.\n"

#define T_DELETE_ROOM_SUCCES \
"Camera de chat a fost stearsa cu succes.\n"

#define T_LOGIN_NAME \
"Nume: "

#define T_LOGIN_PASS \
"Password: "

#define T_ROOM_NAME \
"Nume room: "

#define T_LOGIN_FAIL \
"Login esuat. Utilizatorul nu exista sau parola este incorecta.\n"

#define T_LOGIN_SUCCES \
"Login cu succes! Bine ati (re)venit!\n"

#define T_LOGOUT_FAIL \
"Logout esuat. Nu sunteti conectat.\n"

#define T_LOGOUT_SUCCES \
"Logout cu succes.\n"

#define T_SENT_FRIEND_REQ_SUCCES \
"Cerere de prietenie trimisa.\n"

#define T_SENT_FRIEND_REQ_FAIL \
"Cererea de prietenie nu a fost trimisa. Date gresite.\n"

#define T_POST_TEXT \
"Textul postarii (max. 100 caractere): "

#define T_POST_ID \
"ID-ul postarii: "

#define T_USER_ID \
"ID-ul utilizatorului: "

#define T_USER_TYPE \
"Tipul de prietenie (1 - friends, 2 - close friends): "

#define T_POST_VISIBILITY \
"Vizibilitate postare (0 - public, 1 - friends, 2 - close friends): "

#define T_ADD_POST_FAIL \
"Postarea nu a fost adaugata. Nu sunteti conectat / date incorecte.\n"

#define T_ADD_POST_SUCCES \
"Postare adaugata cu succes!\n"

#define T_DELETE_POST_FAIL \
"Postarea nu a fost stearsa. Nu aveti drepturile necesare / nu este postarea Dvs \
postarea nu exista / nu sunteti conectat.\n"

#define T_DELETE_POST_SUCCES \
"Postarea a fost stearsa cu succes!\n"

#define T_EDIT_POST_FAIL \
"Postarea nu a fost editata. Nu este postarea Dvs / postarea nu exista / nu sunteti conectat.\n"

#define T_EDIT_POST_SUCCES \
"Postarea a fost editata cu succes!\n"

#define T_EDIT_PROFILE_FAIL \
"Profilul nu a fost editat. Date gresite / nu sunteti conectat.\n"

#define T_EDIT_PROFILE_SUCCES \
"Profilul a fost editat cu succes.\n"

#define T_ROOM_ID \
"ID Room: "

#define T_NO_ROOMS \
"Nu sunt camere de chat.\n"

#endif

#ifdef H_CLIENT

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

#define C_CONNECT_SUCCES \
printf("Bine ai venit!! Tasteaza [!help] pentru a vedea instructiunile.\n");

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

#endif


#ifdef H_SERVER

// Mesaje eroare/informare server

#define S_UNLINK_FUNCTIONS \
printf("[SERVER] Functii nelegate corespunzator!\n");

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

#define S_NEW_POST \
printf("[SERVER] Postare noua!\n");

#define S_POST_EDITED \
printf("[SERVER] Postarea cu ID = %d editata\n", idPost);

#define S_NEW_CLIENT \
printf("[SERVER] Client nou!\n");

#define S_QUIT_CLIENT \
printf("[SERVER] Un client a iesit\n");

#endif

#ifdef H_DATABASE

// Mesaje de eroare/informare DB

#define DB_OPEN_MESSAGES_ERROR \
{\
printf("[DATABASE] Eroare la open() - tabela Messages"); \
return false;\
}

#define DB_CREATE_MESSAGES_ERROR \
{\
printf("[DATABASE] Eroare la create table - tabela Messages\n %s\n", err);\
sqlite3_free(err);\
sqlite3_close(db);\
return false;\
}

#define DB_CREATE_MESSAGES_OK \
printf("[DATABASE] Tabela Messages creata cu succes\n");

#define DB_DELETE_MESSAGES_ERROR \
printf("[DATABASE] Eroare la Delete() in Messages\n");

#define DB_SELECT_MESSAGES_ERROR \
printf("[DATABASE] Eroare la Select() in Messages\n");

#define DB_INSERT_MESSAGES_ERROR \
printf("[DATABASE] Eroare la Insert() in Messages\n");

#define DB_ROOM_MEMBERS \
"In total sunt conectati: "

#define DB_OPEN_ROOMS_ERROR \
{\
printf("[DATABASE] Eroare la open() - tabela Rooms"); \
return false;\
}

#define DB_CREATE_ROOMS_ERROR \
{\
printf("[DATABASE] Eroare la create table - tabela Rooms\n %s\n", err);\
sqlite3_free(err);\
sqlite3_close(db);\
return false;\
}

#define DB_CREATE_ROOMS_OK \
printf("[DATABASE] Tabela Rooms creata cu succes\n");

#define DB_DELETE_ROOMS_ERROR \
printf("[DATABASE] Eroare la Delete() Room\n");

#define DB_SELECT_ROOMS_ERROR \
printf("[DATABASE] Eroare la Select() in Rooms\n");

#define DB_INSERT_ROOMS_ERROR \
printf("[DATABASE] Eroare la Insert() in Rooms\n");

#define DB_INSERT_ROOMS_OK \
printf("[DATABASE] Camera noua creata cu succes\n");

#define DB_OPEN_FRIENDSHIPS_ERROR \
{\
printf("[DATABASE] Eroare la open() - tabela Friendships"); \
return false;\
}

#define DB_CREATE_FRIENDSHIPS_ERROR \
{\
printf("[DATABASE] Eroare la create table - tabela Friendships\n %s\n", err);\
sqlite3_free(err);\
sqlite3_close(db);\
return false;\
}

#define DB_CREATE_FRIENDSHIPS_OK \
printf("[DATABASE] Tabela Friendships creata cu succes\n");

#define DB_SELECT_FRIENDSHIPS_ERROR \
printf("[DATABASE] Eroare la Select() din Friendships\n");

#define DB_DELETE_FRIENDSHIPS_ERROR \
printf("[DATABASE] Eroare la Delete() din Friendships\n");

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
printf("[DATABASE] Tabela Users creata cu succes\n");

#define DB_INSERT_USER_OK \
printf("[DATABASE] Userul %s cu parola %s s-a inregistrat cu succes in tabela Users\n", nume, pass);\
printf("[DATABASE] User Id = %d\n", db_nextId);

#define DB_UPDATE_USER_OK \
printf("[DATABASE] Userul cu ID = %d si-a actualizat profilul.\n", userid);

#define DB_CREATE_POST_ERROR \
{\
printf("[DATABASE] Eroare la create table - tabela Posts\n %s\n", err);\
sqlite3_free(err);\
sqlite3_close(db);\
}

#define DB_INSERT_POST_ERROR \
printf("[DATABASE] Eroare la inregistrarea postarii\n");

#define DB_SELECT_POSTS_ERROR \
printf("[DATABASE] Eroare la Select() din Posts\n");

#define DB_DELETE_POST_ERROR \
printf("[DATABASE] Eroare la stergerea unei postari\n");

#define DB_OPEN_POSTS_ERROR \
{\
printf("[DATABASE] Eroare la open() - tabela Posts");\
return false;\
}

#define DB_CREATE_POSTS_OK \
printf("[DATABASE] Tabela Posts creata cu succes\n");

#define DB_SQL_COMMAND \
printf("[DATABASE] SQL Command: %s\n", sql.c_str());

#endif

#endif // H_TEXT