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

/// @brief encoding and set the user's password this is private method, the DB class has access to
/// @param pass unencoded password
void User::setUserPassword(const std::string &pass)
{
    std::string passWord = pass;
    EncodePassword::encodePassword(passWord);
    _pass = passWord;
}

/// @brief copies the already encoded password to the user property ( needed when the object is copied )
///        private method, the DB class has access to
/// @param pass
void User::copyUserPassword(const std::string &pass)
{
    _pass = pass;
}

/// @brief increases the message counter when adding messages to the database (DB)
///        private method, the DB class has access to
void User::addedMesage()
{
    _countMessages++;
    _messageCount = _countMessages;
}

/// @brief To get the number of messages from the user - outgoing/incoming
/// @return int outgoing/incoming count
int User::getMessagesCount() const
{
    return _messageCount;
}

/// @brief Creates a user ID when creating a user
///        private method, the DB class has access to
void User::setCurrentID()
{
    _currentId++;
    _id = _currentId;
}

/// @brief sets the user ID when copying the user
///        private method, the DB class has access to
/// @param id int
void User::setUserID(int id)
{
    _id = id;
}

/// @brief sets the number of messages the user has when copying it
/// @param cout int
void User::setMessageCout(int cout)
{
    _messageCount = cout;
}