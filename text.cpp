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
        case ELoginFail:        strcpy(result, T_LOGIN_FAIL);       break;
        case ELoginSucces:      strcpy(result, T_LOGIN_SUCCES);     break;
        case ELogoutFail:       strcpy(result, T_LOGOUT_FAIL);      break;
        case ELogoutSucces:     strcpy(result, T_LOGOUT_SUCCES);    break;

        default:                strcpy(result, T_UNKNOWN_MESSAGE);  break;
    }
}