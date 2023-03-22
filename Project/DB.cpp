//
// DB.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "DB.h"
#include "EncodePassword.h"

bool DB::addUser(const std::string &name,
                 const std::string &login,
                 const std::string &pass)
{
    if (isUniqueLogin(login))
    {
        User user(name, login);
        user.setUserPassword(pass);
        user.setCurrentID();
        int countMessages = addMessagesToNewUserFromPublicChat(user.getId());
        user.addedMesage(countMessages);
        _userDB.append(user);
        return true;
    }
    return false;
}

bool DB::addUser(User &user)
{
    if (isUniqueLogin(user.getUserLogin()))
    {
        if (user.getId() == 0)
        {
            user.setCurrentID();
        }
        int countMessages = addMessagesToNewUserFromPublicChat(user.getId());
        user.addedMesage(countMessages);
        _userDB.append(user);

        return true;
    }
    return false;
}

void DB::addMessage(Message &message)
{
    if (message.getAuthorID() != 0 && message.getRecipientID() != 0)
    {
        message.isPrivat = true;
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() == message.getAuthorID())
            {
                _userDB[i].addedMesage();
            }

            if (_userDB[i].getId() == message.getRecipientID())
            {
                _userDB[i].addedMesage();
            }
        }
        message.setDateMessage();
        message.setMessageId();
        _messageDB.append(message);
    }
}

void DB::AddMessageToAllUsers(Message &message)
{
    if (message.getAuthorID() != 0)
    {
        message.isPrivat = false;
        message.setDateMessage();
        message.setMessageId();
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() != message.getAuthorID())
            {
                message.setRecipientID(_userDB[i].getId());
            }
            else
            {
                message.setRecipientID(0);
            }
            _userDB[i].addedMesage();
            _messageDB.append(message);
        }
    }
}

bool DB::setUserPassword(int userID, const std::string &pass)
{

    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getId() == userID)
        {
            _userDB[i].setUserPassword(pass);
            return true;
        }
    }
    return false;
}

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

bool DB::isCorrectPassword(int userID, const std::string &pass)
{
    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getId() == userID)
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

const std::unique_ptr<User> DB::getUserByLogin(const std::string &login, bool exception) const
{
    if (_userDB.count() > 0)
    {
        std::unique_ptr<User> user(new User());
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getUserLogin() == login)
            {
                (*user) = _userDB[i];
                return user;
            }
        }
    }
    if (exception)
    {
        throw UserNotFoundException();
    }
    return nullptr;
}

int DB::usersCount() const
{
    return _userDB.count();
}

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

const std::unique_ptr<Message[]> DB::getAllMessageForUserById(int id) const
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

bool DB::updateUserData(const User &user)
{
    if (_userDB.count() > 0 && !(isUniqueLogin(user.getUserLogin())))
    {
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() == user.getId())
            {
                _userDB[i].setUserName(user.getUserName());
                _userDB[i].setUserLogin(user.getUserLogin());
                return true;
            }
        }
    }
    return false;
}

bool DB::deleteUserAccount(User &user)
{
    if (_userDB.count() > 0 && !(isUniqueLogin(user.getUserLogin())))
    {
        if (user.isDeleted())
        {
            return true;
        }
        user.deleteThisData();
        _userDB.operator--(user.getId());
        return true;
    }

    return false;
}

bool DB::deleteMessageById(int Id)
{
    if (_messageDB.count() > 0)
    {
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if (_messageDB[i].getId() == Id)
            {
                int authorId = _messageDB[i].getAuthorID();
                int recipientId = _messageDB[i].getRecipientID();

                for (int i = 0; i < _userDB.count(); i++)
                {
                    if (_userDB[i].getId() == authorId || _userDB[i].getId() == recipientId)
                    {
                        _userDB[i].deletedMessage();
                    }
                }
            }
        }
        _messageDB.deleteById(Id);
        return true;
    }
    return false;
}

// Private methods
bool DB::isUsedId(std::unique_ptr<int[]> &arrayId, int id, int size) const
{
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            if (arrayId[i] == id)
            {
                return true;
            }
        }
    }
    return false;
}

int DB::addMessagesToNewUserFromPublicChat(int id)
{
    int count{0};
    int messageCount = _messageDB.count();
    Message message;

    std::unique_ptr<int[]> arrayId = std::make_unique<int[]>(_messageDB.count());
    if (_messageDB.count() > 0)
    {
        for (int i = 0; i < messageCount; i++)
        {
            if (!(_messageDB[i].isPrivat))
            {
                if (_messageDB[i].getRecipientID() != id && _messageDB[i].getRecipientID() != 0 && !isUsedId(arrayId, _messageDB[i].getId(), count))
                {
                    message = _messageDB[i];
                    message.setRecipientID(id);
                    _messageDB.append(message);
                    arrayId[count] = message.getId();
                    count++;
                }
            }
        }
    }
    return count;
}