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
    User service_admin("admin", "admin", "1234");
    service_admin.setIsAdmin(true);
    db->addUser(service_admin);                                                         // Service Admin
    db->addUser("complaint_bot", "complaint_bot", "uijyh6758r9ifSDFGH&^tf34rdfcvgtre"); // бот которому отсылаются жалобы пользователей.

    ChatUserInterface userInterface;
    ChatAdminInterface adminInterface;

// тестовые сообщения и пользователи
#ifdef test
    db->addUser("Максим", "mmn", "pass");      // 2
    db->addUser("Иван", "ivanKispaj", "pass"); // 3
    db->addUser("Ирина", "Fineralla", "pass"); // 4
    db->addUser("Vasya", "vas", "pass");       // 5

    User u("николай", "nick", "pass");
    u.setIsBanned(true);
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

    msg.setMessage("Надеюсь всё хорошо");
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