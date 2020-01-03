#define H_SERVER

#include "text.h"
#include <cstring>

void ConvertToMessage(EMesaje mesaj, char *result)
{
    strcpy(result, "");

    switch(mesaj)
    {
        case EMHelp:                    strcpy(result, T_HELP);                     break;
        case EMQuit:                    strcpy(result, T_QUIT);                     break;
        case EMRegisterFail:            strcpy(result, T_REGISTER_FAIL);            break;
        case EMRegisterSucces:          strcpy(result, T_REGISTER_SUCCES);          break;
        case EMLoginFail:               strcpy(result, T_LOGIN_FAIL);               break;
        case EMLoginSucces:             strcpy(result, T_LOGIN_SUCCES);             break;
        case EMLogoutFail:              strcpy(result, T_LOGOUT_FAIL);              break;
        case EMLogoutSucces:            strcpy(result, T_LOGOUT_SUCCES);            break;
        case EMAddPostFail:             strcpy(result, T_ADD_POST_FAIL);            break;
        case EMAddPostSucces:           strcpy(result, T_ADD_POST_SUCCES);          break;
        case EMDeletePostFail:          strcpy(result, T_DELETE_POST_FAIL);         break;
        case EMDeletePostSucces:        strcpy(result, T_DELETE_POST_SUCCES);       break;
        case EMEditPostFail:            strcpy(result, T_EDIT_POST_FAIL);           break;
        case EMEditPostSucces:          strcpy(result, T_EDIT_POST_SUCCES);         break;
        case EMEditProfileFail:         strcpy(result, T_EDIT_PROFILE_FAIL);        break;
        case EMEditProfileSucces:       strcpy(result, T_EDIT_PROFILE_SUCCES);      break;
        case EMSentFriendRequestFail:   strcpy(result, T_SENT_FRIEND_REQ_FAIL);     break;
        case EMSentFriendRequestSucces: strcpy(result, T_SENT_FRIEND_REQ_SUCCES);   break;
        case EMRemoveFriendFail:        strcpy(result, T_REMOVE_FRIEND_FAIL);       break;
        case EMRemoveFriendSucces:      strcpy(result, T_REMOVE_FRIEND_SUCCES);     break;
        case EMAcceptRequestFail:       strcpy(result, T_ACCEPT_REQUEST_FAIL);      break;
        case EMAcceptRequestSucces:     strcpy(result, T_ACCEPT_REQUEST_SUCCES);    break;
        case EMRemoveRequestFail:       strcpy(result, T_REMOVE_REQUEST_FAIL);      break;
        case EMRemoveRequestSucces:     strcpy(result, T_REMOVE_REQUEST_SUCCES);    break;
        case EMJoinRoomFail:            strcpy(result, T_JOIN_ROOM_FAIL);           break;
        case EMJoinRoomSucces:          strcpy(result, T_JOIN_ROOM_SUCCES);         break;
        case EMCreateRoomFail:          strcpy(result, T_CREATE_ROOM_FAIL);         break;
        case EMCreateRoomSucces:        strcpy(result, T_CREATE_ROOM_SUCCES);       break;
        case EMMessageFail:             strcpy(result, T_MESSAGE_FAIL);             break;
        case EMLeaveRoomFail:           strcpy(result, T_LEAVE_ROOM_FAIL);          break;
        case EMLeaveRoomSucces:         strcpy(result, T_LEAVE_ROOM_SUCCES);        break;
        case EMDeleteRoomFail:          strcpy(result, T_DELETE_ROOM_FAIL);         break;
        case EMDeleteRoomSucces:        strcpy(result, T_DELETE_ROOM_SUCCES);       break;

        default:                        strcpy(result, T_UNKNOWN_MESSAGE);          break;
    }
}