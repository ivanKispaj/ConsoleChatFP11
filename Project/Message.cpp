//
// Message.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "Message.h"

void Message::setMessage(const std::string &message)
{
    _message = message;
}

void Message::setAuthorID(int authorID)
{
    _authorID = authorID;
}

void Message::setRecipientID(int recipientID)
{
    _recipientID = recipientID;
}

const std::string &Message::getMessage() const
{
    return _message;
}

int Message::getAuthorID() const
{
    return _authorID;
}

int Message::getRecipientID() const
{
    return _recipientID;
}