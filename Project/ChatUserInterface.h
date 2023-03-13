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
    /// @return Input result
    Input run() override;
    Input loginInChat();
    Input registration();
    Input chat();
};