//
// DB.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "DB.h"
#include "EncodePassword.h"

/// @brief added new user to DB
/// @param user struct User type
/// @return true - if is success, false if login non-unique!
bool DB::addUser(const std::string &name,
                 const std::string &login,
                 const std::string &pass)
{
    if (isUniqueLogin(login))
    {
        User user(name, login);
        user.setUserPassword(pass);
        user.setCurrentID();
        _userDB.append(user);
        return true;
    }
    return false;
}

bool DB::addUser(const User &user)
{
    if (isUniqueLogin(user.getUserLogin()))
    {
        User saveUser(user.getUserName(), user.getUserLogin());
        saveUser.copyUserPassword(user.getUserPassword());
        if (user.getUserId() == 0)
        {
            saveUser.setCurrentID();
        }
        else
        {
            saveUser.setUserID(user.getUserId());
        }
        _userDB.append(saveUser);
        return true;
    }
    return false;
}

/// @brief added message to DB
/// @param message struct Message type
void DB::addMessage(const Message &message)
{
    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getUserId() == message.getAuthorID())
        {
            _userDB[i].addedMesage();
        }
    }
    _messageDB.append(message);
}

/// @brief Sets the user account password
/// @param userID int unique id
/// @param pass std::string new password
/// @return true - if is success, false - if there is no user with this id
bool DB::setUserPassword(int userID, const std::string &pass)
{

    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getUserId() == userID)
        {
            _userDB[i].setUserPassword(pass);
            return true;
        }
    }
    return false;
}

/// @brief Checks whether the login is unique.
/// @param login std::string.
/// @return true - if login unique, false - if the login exists.
bool DB::isUniqueLogin(const std::string &login)
{
    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getUserLogin() == login)
        {
            return false;
        }
    }
    return true;
}

/// @brief checks whether passwords match or not
/// @param userID int ID user
/// @param pass std::string incoming password
/// @return true - if the passwords match, false - if the passwords are different
bool DB::isCorrectPassword(int userID, const std::string &pass)
{
    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getUserId() == userID)
        {
            std::string verifyPass = pass;
            EncodePassword::encodePassword(verifyPass);
            if (_userDB[i].getUserPassword() == verifyPass)
            {
                return true;
            }
        }
    }
    return false;
}

/// @brief Returns an array of users
/// @return  if the array is not empty - std::unique_ptr<User[]> , nullptr if the array is empty!
const std::unique_ptr<User[]> DB::getAllUsers() const
{
    if (_userDB.count() > 0)
    {
        std::unique_ptr<User[]> ret(new User[_userDB.count()]);
        for (int i = 0; i < _userDB.count(); i++)
        {
            ret[i] = _userDB[i];
        }
        return std::move(ret);
    }
    return nullptr;
}

/// @brief Returns the size of the user array
/// @return  int user array size
int DB::usersCount() const
{
    return _userDB.count();
}

const std::unique_ptr<User> DB::getUserByLogin(const std::string &login) const
{

    if (_userDB.count() > 0)
    {
        std::unique_ptr<User> user(new User());
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getUserLogin() == login)
            {
                (*user).setUserLogin(_userDB[i].getUserLogin());
                (*user).setUserName(_userDB[i].getUserName());
                (*user).setUserPassword(_userDB[i].getUserPassword());
                (*user).setUserID(_userDB[i].getUserId());
                (*user).setMessageCout(_userDB[i].getMessagesCount());
                return user;
            }
        }
    }
    return nullptr;
}

/// @brief Returns an array of all messages
/// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
const std::unique_ptr<Message[]> DB::getAllMessage() const
{
    if (_messageDB.count() > 0)
    {
        std::unique_ptr<Message[]> ret(new Message[_messageDB.count()]);
        for (int i = 0; i < _messageDB.count(); i++)
        {
            ret[i] = _messageDB[i];
        }
        return std::move(ret);
    }
    return nullptr;
}

/// @brief Returns an array of all messages for authorID
/// @param authorID int unique author ID
/// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
const std::unique_ptr<Message[]> DB::getAllMessageByAuthorID(int authorID) const
{
    DBCore<Message> newMessageArray;
    if (_messageDB.count() > 0)
    {

        bool issetMessage = false;
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if (_messageDB[i].getAuthorID() == authorID)
            {
                newMessageArray.append(_messageDB[i]);
                issetMessage = true;
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }

            return std::move(ret);
        }
    }
    return nullptr;
}

/// @brief Returns an array of all messages for recipientID
/// @param recipientID int unique recipientID
/// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
const std::unique_ptr<Message[]> DB::getAllUserMessageByID(int id) const
{
    DBCore<Message> newMessageArray;

    if (_messageDB.count() > 0)
    {
        bool issetMessage = false;
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if ((_messageDB[i].getAuthorID() == id) || (_messageDB[i].getRecipientID() == id))
            {
                newMessageArray.append(_messageDB[i]);
                issetMessage = true;
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }

            return std::move(ret);
        }
    }
    return nullptr;
}
