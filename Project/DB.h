//
// DB.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once

#include "DBCore.h"
#include "User.h"
#include "Message.h"

class DB
{
    DBCore<User> _userDB;
    DBCore<Message> _messageDB;

public:
    DB() = default;
    ~DB() = default;
    bool addUser(const std::string &name,
                 const std::string &login,
                 const std::string &pass);
    bool addUser(User &user);
    void addMessage(const Message &message);
    bool setUserPassword(int userID, const std::string &pass);
    bool isUniqueLogin(const std::string &login);
    bool isCorrectPassword(int userID, const std::string &pass);
    const std::unique_ptr<User[]> getAllUsers() const;
    const std::unique_ptr<User> getUserByLogin(const std::string &login, bool exception = false) const;
    int usersCount() const;
    // Get all messages
    const std::unique_ptr<Message[]> getAllMessage() const;
    // Get all user messages by login and user messages from senders in chronological ascending order
    const std::unique_ptr<Message[]> getAllUserMessageByID(int id) const;
    // Get all user messages by User ID
    const std::unique_ptr<Message[]> getAllMessageByAuthorID(int authorID) const;
    // Updates the user's data (name, login), except for the password!
    bool updateUserData(const User &user);
    bool deleteUserAccount(User &user); // true -> if accaunt deleted, false -> failed
};