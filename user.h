#ifndef USER_H
#define USER_H

#include "utils.h"

class CUser {
    static CId m_CountId;

    public:
        CUser();
        Reset();

        string  GetName()   const;
        CId     GetId()     const;

        bool SetName(const string& name);
        bool TryLogin(const string& password);
        bool IsLogged() const;

        bool AddFriend(const CId& idUser);
        bool RemoveFriend(const CId& idUser);

        bool AddPost(const CId& idPost, const string& message);
        bool RemovePost(const CId& idPost);

    private:
        CId             m_Id;
        string          m_Name;
        bool            m_IsVisitor;
        /// To-Do: array of posts
};


#endif //USER_H
