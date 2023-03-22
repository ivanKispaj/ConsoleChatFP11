#pragma once
#include <iostream>
#include "IChatInterface.h"
#include "ChatUserInterface.h"
#include "ChatAdminInterface.h"
#include "Misc.h"
#include "UserInput.h"
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

    Results userInput;
    Results result = empty;
    // Объект страницы
    UserInput<std::string, Results> coreAreaPage("Главная станица",
                                             "Выберите действия: ч - Чат, а - Раздел администратора, в - Выход из программы ",
                                             "Неверный ввод",
                                             3 // количество возможных вариантов ввода
    );

    // создание возможных вариантов ввода
    coreAreaPage.addInputs("ч", "а", "в");
    // Соответствующие варианты вывода
    coreAreaPage.addOutputs(Results::chat, Results::admin, Results::app_exit);

    do
    {
        userInput = coreAreaPage.IOAction();
        switch (userInput)
        {
        case Results::chat:
            result = userInterface.run(std::move(db));
            db = std::move(userInterface.db);
            break;
        case Results::admin:
            result = adminInterface.run(std::move(db));
            db = std::move(adminInterface.db);
            break;
        case Results::app_exit:
            result = Results::app_exit;
            break;
        default:
            std::cout << "Неверный ввод" << std::endl;
            break;
        }
    } while (result != Results::app_exit);
    std::cout << "Вы вышли из чата. До свидания." << std::endl;
}