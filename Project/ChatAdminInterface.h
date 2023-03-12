#pragma once
#include "IChatInterface.h"
class ChatAdminInterface final : public IChatInterface
{
private:
    /* data */
public:
    ChatAdminInterface() = default;
    ChatAdminInterface(DB *_db);
    ~ChatAdminInterface() = default;
    int run() override;
};