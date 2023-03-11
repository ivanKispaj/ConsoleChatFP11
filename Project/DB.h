//
// DB.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once

#include "DBCore.h"
#include "User.h"
#include "Message.h"
#include <iostream>

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
    bool addUser(const User &user);
    void addMessage(const Message &message);
    bool setUserPassword(int userID, const std::string &pass);
    bool isUniqueLogin(const std::string &login);
    bool isCorrectPassword(int userID, const std::string &pass);
    const std::unique_ptr<User[]> getAllUsers() const;
    const std::unique_ptr<User> getUserByLogin(const std::string &login) const;
    int usersCount() const;
    // Get all messages
    const std::unique_ptr<Message[]> getAllMessage() const;
    // Get all user messages by login and user messages from senders in chronological ascending order
    const std::unique_ptr<Message[]> getAllUserMessageByID(int id) const;
    // Get all user messages by User ID
    const std::unique_ptr<Message[]> getAllMessageByAuthorID(int authorID) const;
};