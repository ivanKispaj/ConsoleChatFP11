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
    /// @return Results
    Results run(std::unique_ptr<DB> _db) override;
    Results loginInChat();
    Results registration();
    Results chat();
    void sendMessage();
};