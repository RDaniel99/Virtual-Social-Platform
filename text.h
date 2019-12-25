#define T_UNKNOWN_MESSAGE \
"Mesaj necunoscut\n"

#define T_HELP \
"Comenzi:\n \
!help - Afiseaza acest mesaj.\n \
!quit - Iesire.\n"

#define T_QUIT \
"O zi buna! Va mai asteptam\n"


#define S_WRITE_ERROR \
perror("[SERVER] Eroare la write()\n"); \
exit(1)

enum EMesaje
{
    EUnknown = 0,
    EHelp,
    EQuit
};

void ConvertToMessage(EMesaje mesaj, char *result);