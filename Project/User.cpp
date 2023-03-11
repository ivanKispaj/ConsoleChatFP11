//
// User.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "User.h"
#include "DB.h"
#include "EncodePassword.h"
int User::_currentId;
int User::_countMessages;

/// @brief init
/// @param name string name
/// @param login string login unique
/// @param pass string password
User::User(const std::string &name, const std::string &login, const std::string &pass)
{
    _name = name;
    _login = login;
    setUserPassword(pass);
    setCurrentID();
}