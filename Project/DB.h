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
    DBCore<User> _userDB;       // database User
    DBCore<Message> _messageDB; // database Message

    /// @brief private method : Adds public messages to newly registered users
    /// @param id int user id
    /// @return
    int addMessagesToNewUserFromPublicChat(int id);

    /// @brief private method :
    /// @param arrayId Array of numbers
    /// @param id int The number to check
    /// @param size Array size
    /// @return true If the "id" is present in the array
    bool isUsedId(std::unique_ptr<int[]> &arrayId, int id, int size) const;

public:
    /// @brief default init
    DB() = default;

    /// @brief default deinit
    ~DB() = default;

    /// @brief adds a new user to the database
    /// @param name string
    /// @param login string
    /// @param pass string -> unencoded password
    /// @return true - if is success, false if login non-unique!
    bool addUser(const std::string &name,
                 const std::string &login,
                 const std::string &pass);

    /// @brief adds a new user to the database
    /// @param user struct User
    /// @return true - if is success, false if login non-unique!
    bool addUser(User &user);

    /// @brief adds a message to the database in a private chat
    /// @param message struct Message type
    void addMessage(Message &message);

    /// @brief adds a message to the database in a public chat, for all users
    /// @param message
    void AddMessageToAllUsers(Message &message);

    /// @brief Sets the user account password
    /// @param userID int unique id
    /// @param pass std::string new password
    /// @return true - if is success, false - if there is no user with this id
    bool setUserPassword(int userID, const std::string &pass);

    /// @brief Checks whether the login is unique.
    /// @param login std::string.
    /// @return true - if login unique, false - if the login exists.
    bool isUniqueLogin(const std::string &login);

    /// @brief checks whether passwords match or not
    /// @param userID int ID user
    /// @param pass std::string incoming password
    /// @return true - if the passwords match, false - if the passwords are different
    bool isCorrectPassword(int userID, const std::string &pass);

    /// @brief Returns an array of users
    /// @return  if the array is not empty - std::unique_ptr<User[]> , nullptr if the array is empty!
    const std::unique_ptr<User[]> getAllUsers() const;

    /// @brief Get a user by login
    /// @param login string -> user login
    /// @param exception
    /// @return Returning a pointer to a user object
    const std::unique_ptr<User> getUserByLogin(const std::string &login, bool exception = false) const;

    /// @brief Returns the size of the user array
    /// @return  int user array size
    int usersCount() const;

    /// @brief Returns an array of all messages
    /// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
    const std::unique_ptr<Message[]> getAllMessage() const;

    /// @brief Returns an array of all incoming/outgoing messages for - user with ID, in chronological ascending order
    /// @param id int unique recipientID
    /// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
    const std::unique_ptr<Message[]> getAllMessageForUserById(int id) const;

    /// @brief Updates the user's data (name, login), except for the password!
    /// @param user struct User
    /// @return true if successful, false if not a unique login, no user, no users
    bool updateUserData(const User &user);

    /// @brief Deletes the user's account, marked as deleted. The password is reset, the name is changed to the "deleted"
    /// @param user struct User
    /// @return true if successful, false if not a unique login, no user, no users
    bool deleteUserAccount(User &user); // true -> if accaunt deleted, false -> failed

    /// @brief
    /// @param message
    /// @return
    bool deleteMessageById(int Id);
};