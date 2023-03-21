//
// User.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once
#include <string>
#define private      \
    friend class DB; \
private

struct User
{
private:
    std::string _name;
    std::string _login;
    std::string _pass;
    static int _countMessages;
    int _messageCount{0};
    int _id{0};
    static int _currentId;
    bool _isAdmin{false};   // true if the user is an admin
    bool _isBanned{false};  // true if the user is an bannded
    bool _isDeleted{false}; // true if the user is an deleted account

#undef private                                      // private method (friend to DB class)
    void setUserPassword(const std::string &pass);  // sets the user's password in encoded form
    void copyUserPassword(const std::string &pass); // copy encoded user password
    void setMessageCout(int cout);                  // sets the number of messages when copying
    void addedMesage();                             // added message count
    void setCurrentID();                            // Creates a user ID when creating a user
    void setUserID(int id);                         // sets the user ID when copying the user

public:
    User() = default; // default init without parameters
    ~User() = default;
    User(const std::string &name, const std::string &login) : _name(name), _login(login) {}
    User(const std::string &name, const std::string &login, const std::string &pass);
    int getId() const;                           // return the user's ID
    const std::string &getUserName() const;      // return the user's name
    const std::string &getUserLogin() const;     // return the user's login
    const std::string &getUserPassword() const;  // return the user's password in encoded form
    void setUserName(const std::string &name);   // set the user's name
    void setUserLogin(const std::string &login); // set the user's login
    int getMessagesCount() const;                // return count message
    bool isAdmin() const;                        // return  true -> user is Admin, false -> user is not Admin
    bool isBanned() const;                       // return true -> user as Banned, false -> user as not Banned
    bool isDeleted() const;                      // return true -> user account is deleted
    void setIsAdmin(bool isAdmin);               // set true if the user is an admin, or it will false
    void setIsBannded(bool isBanned);            // true if the user is an banned, or it will false
    void deleteThisData();                       // Erases user data , except for ID and Login, or it will false
    User &operator=(const User &user);
};
