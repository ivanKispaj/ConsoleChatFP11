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

#undef private
    void setUserPassword(const std::string &pass);  // private method (friend to DB class) sets the user's password in encoded form
    void copyUserPassword(const std::string &pass); // copy encoded user password
    void setMessageCout(int cout);                  // sets the number of messages when copying

public:
    User() = default; // default init without parameters
    ~User() = default;
    User(const std::string &name, const std::string &login) : _name(name), _login(login) {}
    User(const std::string &name, const std::string &login, const std::string &pass);
    int getUserId() const;                       // return the user's ID
    const std::string &getUserName() const;      // return the user's name
    const std::string &getUserLogin() const;     // return the user's login
    const std::string &getUserPassword() const;  // return the user's password in encoded form
    void setUserName(const std::string &name);   // set the user's name
    void setUserLogin(const std::string &login); // set the user's login
    void addedMesage();                          // added message count
    int getMessagesCount() const;                // return count message
    void setCurrentID();
    void setUserID(int id);
};
