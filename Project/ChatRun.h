#pragma once
#include <iostream>
#include "IChatInterface.h"
#include "ChatUserInterface.h"
#include "ChatAdminInterface.h"
#include "Misc.h"
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
    User serviceAdmin("admin", "admin", "1234");
    db->addUser(serviceAdmin);

    ChatUserInterface userInterface(db);
    ChatAdminInterface adminInterface(db);
    char userInput = 'c';
    Input inputStatus = empty;

    int exitCode = 0;
    do
    {
        userInput = getInput<char>("Выберите действие (с - войти в чат, a - войти в раздел администратора, e - выход из программы): ");
        switch (userInput)
        {
        case 'c':
            inputStatus = userInterface.run();
            break;

        case 'a':
            inputStatus = adminInterface.run();
            break;

        case 'e':
            return;

        default:
            std::cout << "Неверный ввод" << std::endl;
            break;
        }
    } while (inputStatus != Input::close);
}