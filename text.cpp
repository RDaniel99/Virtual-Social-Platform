#include "text.h"
#include <cstring>

void ConvertToMessage(EMesaje mesaj, char *result)
{
    strcpy(result, "");

    switch(mesaj)
    {
        case EHelp: strcpy(result, T_HELP);             break;
        case EQuit: strcpy(result, T_QUIT);             break;

        default:    strcpy(result, T_UNKNOWN_MESSAGE);  break;
    }
}