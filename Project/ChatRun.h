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
    std::unique_ptr<DB> db = std::make_unique<DB>();
    User serviceAdmin("admin", "admin", "1234");
    db->addUser(serviceAdmin);

    ChatUserInterface userInterface;
    ChatAdminInterface adminInterface;

    char userInput;
    Results result = empty;

    do
    {
        userInput = getInput<char>("Выберите действие (с - войти в чат, a - войти в раздел администратора, e - выход из программы): ");
        switch (userInput)
        {
        case 'c':
            result = userInterface.run(std::move(db));
            db = std::move(userInterface.db);
            break;
        case 'a':
            result = adminInterface.run(std::move(db));
            db = std::move(adminInterface.db);
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