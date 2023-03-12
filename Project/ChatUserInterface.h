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
    /// @return int, 0 - выход из программы, 1 - Выйти из чата
    int run() override;

    int registration();
    int login();
    int chat();
};