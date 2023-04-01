#pragma once
#include <iostream>
#include "IChatInterface.h"
#include "ChatUserInterface.h"
#include "ChatAdminInterface.h"
#include "UserInput.h"
#define test(a) ;

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
    db->addUser("admin", "admin", "1234"); // ServiceAdmin

    ChatUserInterface userInterface;
    ChatAdminInterface adminInterface;

// тестовые сообщения и пользователи
#ifdef test
    db->addUser("Максим", "mmn", "pass");      // 2
    db->addUser("Иван", "ivanKispaj", "pass"); // 3
    db->addUser("Ирина", "Fineralla", "pass"); // 4
    db->addUser("Vasya", "vas", "pass");       // 5

    User u("николай", "nick", "pass");
    u.setIsBannded(true);
    db->addUser(u);
    
    Message msg;
    msg.setMessage("Всем привет!");
    msg.setAuthorID(2);
    db->AddMessageToAllUsers(msg);

    msg.setMessage("И тебе привет 👋!");
    msg.setAuthorID(3);
    db->AddMessageToAllUsers(msg);

    msg.setMessage("Как у нас дела? 🔥 ;-))))))");
    msg.setAuthorID(4);
    db->AddMessageToAllUsers(msg);

    msg.setMessage("Как у нас дела? 🔴 ;-))))))");
    db->AddMessageToAllUsers(msg);
#endif

    chat::Results userInput = chat::empty;
    chat::Results result = chat::empty;
    // Объект страницы
    UserInput<std::string, chat::Results> coreAreaPage("Главная станица",
                                                       "Выберите действия: ч - Чат, а - Раздел администратора, в - Выход из программы ",
                                                       "Неверный ввод",
                                                       3 // количество возможных вариантов ввода
    );

    // создание возможных вариантов ввода
    coreAreaPage.addInputs("ч", "а", "в");
    // Соответствующие варианты вывода
    coreAreaPage.addOutputs(chat::chat, chat::admin, chat::app_exit);

    do
    {
        userInput = coreAreaPage.IOgetline();
        switch (userInput)
        {
        case chat::chat:
            system(clear);
            result = userInterface.run(std::move(db));
            db = std::move(userInterface.db);
            break;
        case chat::admin:
            result = adminInterface.run(std::move(db));
            db = std::move(adminInterface.db);
            break;
        case chat::app_exit:
            result = chat::app_exit;
            break;
        default:
            std::cout << "Неверный ввод" << std::endl;
            break;
        }
    } while (result != chat::app_exit);
    std::cout << "Вы вышли из чата. До свидания." << std::endl;
}