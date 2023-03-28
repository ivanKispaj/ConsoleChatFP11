#pragma once
#include "IChatInterface.h"
class ChatAdminInterface final : public IChatInterface
{
private:
    /* data */
public:
    ChatAdminInterface() = default;
    ~ChatAdminInterface() = default;
    chat::Results run(std::unique_ptr<DB> _db) override;
};