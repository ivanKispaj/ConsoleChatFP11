#pragma once
#include <iostream>
#include "IChatInterface.h"
#include "ChatUserInterface.h"
#include "ChatAdminInterface.h"

class ChatRun
{
private:
public:
    ChatRun();
    ~ChatRun() = default;
};

ChatRun::ChatRun()
{
    DB *db = new DB();
    ChatUserInterface userInterface(db);
    ChatAdminInterface adminInterface(db);

    char userInput = 'c';
    int exitCode = 0;
    do
    {
        std::cout << "Выберите действие (с - войти в чат, a - войти в раздел администратора, e - выход из программы): ";
        std::cin >> userInput;
        switch (userInput)
        {
        case 'c':
            exitCode = userInterface.run();
            break;

        case 'a':
            exitCode = adminInterface.run();
            break;

        case 'e':
            return;

        default:
            std::cout << "Неверный ввод" << std::endl;
            break;
        }
    } while (exitCode);
}