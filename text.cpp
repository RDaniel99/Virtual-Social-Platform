#define H_SERVER

#include "text.h"
#include <cstring>

void ConvertToMessage(EMesaje mesaj, char *result)
{
    strcpy(result, "");

    switch(mesaj)
    {
        case EMHelp:                strcpy(result, T_HELP);                 break;
        case EMQuit:                strcpy(result, T_QUIT);                 break;
        case EMRegisterFail:        strcpy(result, T_REGISTER_FAIL);        break;
        case EMRegisterSucces:      strcpy(result, T_REGISTER_SUCCES);      break;
        case EMLoginFail:           strcpy(result, T_LOGIN_FAIL);           break;
        case EMLoginSucces:         strcpy(result, T_LOGIN_SUCCES);         break;
        case EMLogoutFail:          strcpy(result, T_LOGOUT_FAIL);          break;
        case EMLogoutSucces:        strcpy(result, T_LOGOUT_SUCCES);        break;
        case EMAddPostFail:         strcpy(result, T_ADD_POST_FAIL);        break;
        case EMAddPostSucces:       strcpy(result, T_ADD_POST_SUCCES);      break;
        case EMDeletePostFail:      strcpy(result, T_DELETE_POST_FAIL);     break;
        case EMDeletePostSucces:    strcpy(result, T_DELETE_POST_SUCCES);   break;
        case EMEditPostFail:        strcpy(result, T_EDIT_POST_FAIL);       break;
        case EMEditPostSucces:      strcpy(result, T_EDIT_POST_SUCCES);     break;
        case EMEditProfileFail:     strcpy(result, T_EDIT_PROFILE_FAIL);    break;
        case EMEditProfileSucces:   strcpy(result, T_EDIT_PROFILE_SUCCES);  break;

        default:                    strcpy(result, T_UNKNOWN_MESSAGE);      break;
    }
}