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
    
    ChatAdminInterface adminInterface(db);
    ChatUserInterface userInterface(db);

    char userInput;
    Results result = empty;

    do
    {
        userInput = getInput<char>("Выберите действие (с - войти в чат, a - войти в раздел администратора, e - выход из программы): ");
        switch (userInput)
        {
        case 'c':
            result = userInterface.run();
            break;
        case 'a':
            result = adminInterface.run();
            break;
        case 'e':
            result = app_exit;
            break;
        default:
            std::cout << "Неверный ввод" << std::endl;
            break;
        }
    } while (result != Results::app_exit);
}