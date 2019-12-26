#include "text.h"
#include <cstring>

void ConvertToMessage(EMesaje mesaj, char *result)
{
    strcpy(result, "");

    switch(mesaj)
    {
        case EHelp:             strcpy(result, T_HELP);             break;
        case EQuit:             strcpy(result, T_QUIT);             break;
        case ERegisterFail:     strcpy(result, T_REGISTER_FAIL);    break;
        case ERegisterSucces:   strcpy(result, T_REGISTER_SUCCES);  break;

        default:                strcpy(result, T_UNKNOWN_MESSAGE);  break;
    }
}