//
// Message.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once
#include <iostream>

struct Message
{

    bool isPrivat{false};
    Message() = default;
    ~Message() = default;
    Message(const std::string message, int authorID, int recipientID, bool privateChat = false) : isPrivat(privateChat),
                                                                                                  _message(message),
                                                                                                  _authorID(authorID),
                                                                                                  _recipientID(recipientID) {}
    void setMessage(const std::string &message);
    void setAuthorID(int authorID);
    void setRecipientID(int recipientID);
    const std::string &getMessage() const;
    int getAuthorID() const;
    int getRecipientID() const;

private:
    std::string _message;
    int _authorID = 0;
    int _recipientID = 0;
};
