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

/// @brief  public method
/// @return return the user's ID
int User::getUserId() const
{
    return _id;
}

/// @brief public method
/// @return  return the user's name
const std::string &User::getUserName() const
{
    return _name;
}

/// @brief public method
/// @return // return the user's login
const std::string &User::getUserLogin() const
{
    return _login;
}

/// @brief public method
/// @return return the user's password in encoded form
const std::string &User::getUserPassword() const
{
    return _pass;
}

/// @brief  set the user's name
/// @param name string
void User::setUserName(const std::string &name)
{
    _name = name;
}

/// @brief set the user's login
/// @param login string
void User::setUserLogin(const std::string &login)
{
    _login = login;
}

/// @brief encoding and set the user's password
/// @param pass unencoded password
void User::setUserPassword(const std::string &pass)
{
    std::string passWord = pass;
    EncodePassword::encodePassword(passWord);
    _pass = passWord;
}