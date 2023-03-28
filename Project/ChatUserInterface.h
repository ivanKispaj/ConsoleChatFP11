#pragma once
#include "IChatInterface.h"
#include <iostream>

class ChatUserInterface final : public IChatInterface
{
private:
public:
    ChatUserInterface() = default;
    ~ChatUserInterface() = default;

    /// @brief Первоначальный запуск чата
    /// @return chat::Results
    chat::Results run(std::unique_ptr<DB> _db) override;
    chat::Results loginInChat();
    chat::Results registration();
    chat::Results publicChat();
    void sendMessage();
    
    chat::Results privateChat();
};