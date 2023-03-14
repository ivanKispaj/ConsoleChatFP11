#pragma once
#include "IChatInterface.h"
#include <iostream>
class ChatUserInterface final : public IChatInterface
{
private:
public:
    ChatUserInterface(DB *_db);
    ~ChatUserInterface() = default;

    /// @brief Первоначальный запуск чата
    /// @return Results
    Results run() override;
    Results loginInChat();
    Results registration();
    Results chat();
};