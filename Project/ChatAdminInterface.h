#pragma once
#include "IChatInterface.h"
class ChatAdminInterface final : public IChatInterface
{
private:
    std::unique_ptr<User> complaintBot = nullptr;

public:
    ChatAdminInterface() = default;
    ~ChatAdminInterface() = default;
    chat::Results run(std::unique_ptr<DB> _db) override;

    void mainPage();
    void complaintManage();
    void messagesManage();
    void usersManage();
    void deleteMessage(bool complaint_only = false);
    void userBan(bool ban = true);
    void userSetAdmin(bool adm = true);
};