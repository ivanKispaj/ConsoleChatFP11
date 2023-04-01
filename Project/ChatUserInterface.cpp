#include "ChatUserInterface.h"

chat::Results ChatUserInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    chat::Results userInput = chat::empty;
    UserInput<std::string, chat::Results> chatAreaPage("Страница авторизации и регистрации",
                                                       "Выберите действие (р - Регистрация, вх - Вход, н - Назад, вых - выход): ",
                                                       "Неверный ввод", 4);
    chatAreaPage.addInputs("р", "вх", "н", "вых");
    chatAreaPage.addOutputs(chat::registration, chat::login, chat::back, chat::app_exit);
    do
    {
        system(clear);
        userInput = chatAreaPage.IOgetline();
        switch (userInput)
        {
        case chat::registration:
            userInput = registration();
            break;
        case chat::login:
            userInput = loginInChat();
            break;
        case chat::back:
            userInput = chat::back;
            user = nullptr;
            break;
        case chat::app_exit:
            userInput = chat::app_exit;
            user = nullptr;
            break;
        default:
            break;
        }
        pgDefault();
    } while (userInput != chat::app_exit && userInput != chat::back);
    return userInput;
}

chat::Results ChatUserInterface::loginInChat()
{
    auto result = login();
    if (result == chat::login_success)
    {
        return publicChat();
    }
    return result;
}

chat::Results ChatUserInterface::registration()
{
    std::string login;
    std::string name;
    std::string password;

    std::string incorrectInput = "Неверный ввод. Пустые значения недопустимы.";
    std::cout << "Вы находитесь в меню регистрации" << std::endl;
    UserInput<std::string, std::string> getLogin("Страница регистрации", "Введите логин: ", incorrectInput);
    UserInput<std::string, std::string> getName(std::string(), "Введите отображаемое имя пользователя: ", incorrectInput);
    UserInput<std::string, std::string> getPass(std::string(), "Введите пароль: ", incorrectInput);
    UserInput<std::string, chat::Results> regEnd(std::string(), "Выберите действие (з - Зарегистрироваться, о - Отменить регистрацию): ", "Неверный ввод", 2);
    regEnd.addInputs("з", "о");
    regEnd.addOutputs(chat::register_success, chat::register_cancel);

    // ввод логина
    bool validLogin = false;
    do
    {
        login = getLogin.IOgetlineThrough(true);

        validLogin = db->isUniqueLogin(login);
        if (!validLogin)
        {
            std::cout << "Этот логин занят!" << std::endl;
        }
    } while (!validLogin);

    // ввод пароля
    password = getPass.IOgetlineThrough(true);

    // ввод имени
    name = getName.IOgetlineThrough(true);

    // завершение регистрации
    chat::Results endInput;
    endInput = regEnd.IOgetline();
    if (endInput == chat::register_cancel)
    {
        return chat::register_cancel;
    }

    // При удачном завершении регистрации - переход в чат
    User _user(name, login, password);
    db->addUser(_user);
    user = std::make_unique<User>(_user);
    return publicChat();
}

chat::Results ChatUserInterface::publicChat()
{
    system(clear);
    std::cout << std::endl;
    std::cout << "Здравствуйте, " << user->getUserName() << "!" << std::endl;
    std::cout << "Вы успешно вошли в чат." << std::endl;
    std::cout << std::endl;
    std::string chatDescription;
    std::string mainMessage;

    mainMessage = "Выберите действие: "
                  "(с - написать сообщение; "
                  "н - настройки; "
                  "л - личные сообщения; "
                  "в - выход): ";

    UserInput<std::string, chat::Results> chatMainPage(chatDescription, mainMessage, "Неверный ввод", 4);
    chatMainPage.addInputs("с", "н", "л", "в");
    chatMainPage.addOutputs(chat::send_message, chat::chat_options, chat::private_chat, chat::back);

    chat::Results result = chat::empty;

    do
    {
        auto messages = db->getAllPublicMessages(pg_MaxItems);

        if (messages == nullptr)
        {
            std::cout << "В этом чате нет сообщений. Начните общение первым." << std::endl;
        }
        messagesList(std::move(messages));

        std::cout << std::endl;
        chatDescription = user->getUserName() + " [" + user->getUserLogin() +
                          "] Общий чат. Показаны сообщения: " +
                          std::to_string((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + " - " +
                          std::to_string(pg_EndItem) + " из " +
                          std::to_string(pg_MaxItems);
        chatMainPage.setDescription(chatDescription);

        result = chatMainPage.IOgetline();
        switch (result)
        {
        case chat::send_message:
            sendMessage();
            break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::private_chat:
            result = privateChat();
            break;
        default:
            break;
        }
        system(clear);
    } while (result != chat::back);
    return chat::empty;
}

void ChatUserInterface::sendMessage(chat::Results result)
{
    Message message;
    std::string messageText;
    UserInput<std::string, std::string> send(std::string(), "Введите текст сообщения: ", "Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);

    message.setAuthorID(user->getId());
    message.setMessage(messageText);
    if (result == chat::public_chat)
    {
        db->AddMessageToAllUsers(message);
        return;
    }
    if (result == chat::private_chat)
    {
        message.setRecipientID(pm_user->getId());
        db->addMessage(message);
    }
}

chat::Results ChatUserInterface::privateChat()
{
    system(clear);
    pgDefault();
    UserInput<std::string, chat::Results> privateMainPage("Личные сообщения. Главная страница.",
                                                          "Выбор пользователя:\n"
                                                          "\tл - по логину;\n"
                                                          "\tи - по userID;\n"
                                                          "Другие опции:\n"
                                                          "\tн - навигация по списку пользователей;\n"
                                                          "\tо - вернуться в общий чат;\n"
                                                          "\tв - выйти из чата\n"
                                                          "Укажите опцию: ",
                                                          "Неверный ввод", 5);
    privateMainPage.addInputs("л", "и", "н", "о", "в");
    privateMainPage.addOutputs(chat::search_user_byLogin,
                               chat::search_user_byId,
                               chat::user_list,
                               chat::public_chat,
                               chat::back);
    chat::Results result = chat::private_chat;
    std::string chatDescription;
    do
    {
        if (result == chat::public_chat)
        {
            return chat::public_chat;
        }
        system(clear);
        auto users = db->getAllUsers();
        if (users == nullptr)
        {
            std::cout << "В этом чате нет пользователей. Пригласите других участников." << std::endl;
        }

        usersList(std::move(users));

        if (result == chat::user_not_found)
        {
            std::cout << "Пользователь не найден." << std::endl;
        }
        chatDescription = "Личные сообщения. Главная страница.\n"
                          "Вы: " +
                          user->getUserName() + " [" + user->getUserLogin() + "]\t[userID " + std::to_string(user->getId()) + "]\n" +
                          "Показаны пользователи: " +
                          std::to_string((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + " - " +
                          std::to_string(pg_EndItem) + " из " +
                          std::to_string(pg_MaxItems);

        privateMainPage.setDescription(chatDescription);
        result = privateMainPage.IOgetline();

        switch (result)
        {
        case chat::search_user_byLogin:
            result = privateChatWithUser(chat::search_user_byLogin);
            break;
        case chat::search_user_byId:
            result = privateChatWithUser(chat::search_user_byId);
            break;
        case chat::user_list:
            pgNavigation();
            break;
        case chat::public_chat:
            result = chat::public_chat;
            break;
        case chat::back:
            result = chat::back;
            break;
        default:
            break;
        }
    } while ((result != chat::public_chat) && (result != chat::back));

    pgDefault();
    pm_user = nullptr;
    return result;
}

void ChatUserInterface::searchUser(chat::Results results)
{
    UserInput<int, int> searchByIdInput("Писк пользователя по userID", "Введите userID: ", "Неверный ввод");
    UserInput<std::string, std::string> searchByLoginInput("Писк пользователя по логину", "Введите логин пользователя: ", "Неверный ввод");
    if (results == chat::search_user_byId)
    {
        int uid = searchByIdInput.IOcinThrough();
        std::unique_ptr<User> _user = db->getUserById(uid);
        pm_user = std::move(_user);
    }
    if (results == chat::search_user_byLogin)
    {
        std::string login = searchByLoginInput.IOgetlineThrough();
        std::unique_ptr<User> _user = db->getUserByLogin(login);
        pm_user = std::move(_user);
    }
}

chat::Results ChatUserInterface::privateChatWithUser(chat::Results result)
{
    searchUser(result);
    if (pm_user == nullptr)
    {
        return chat::user_not_found;
    }

    system(clear);
    pgDefault();
    std::string chatDescription;
    std::string mainMessage = "Выберите действие: "
                              "(с - написать сообщение; "
                              "н - навигация по сообщениям; "
                              "п - вернуться публичный чат; "
                              "в - выход из беседы): ";

    UserInput<std::string, chat::Results> chatConversationPage(chatDescription, mainMessage, "Неверный ввод", 4);
    chatConversationPage.addInputs("с", "н", "п", "в");
    chatConversationPage.addOutputs(chat::send_message, chat::chat_options, chat::public_chat, chat::back);

    do
    {
        auto messages = db->getAllPrivateMessagesForUsersById(user->getId(), pm_user->getId(), pg_MaxItems);
        messagesList(std::move(messages));
        if (pg_MaxItems <= 0)
        {
            std::cout << "В этом чате нет сообщений. Начните общение первым." << std::endl;
        }

        chatDescription = "Личные сообщения. Беседа с пользователем.\n"
                          "Вы:\t" +
                          user->getUserName() + " [" + user->getUserLogin() + "]\t[userID " + std::to_string(user->getId()) + "]\n" +
                          "Ваш собеседник:\t" + pm_user->getUserName() + " [" + pm_user->getUserLogin() + "]\t[userID " + std::to_string(pm_user->getId()) + "]\n" +
                          std::string((user->getId() == pm_user->getId()) ? "Вы ведете личную беседу с самим собой.\n" : std::string()) +
                          "Показаны сообщения: " +
                          std::to_string((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + " - " +
                          std::to_string(pg_EndItem) + " из " +
                          std::to_string(pg_MaxItems);
        chatConversationPage.setDescription(chatDescription);
        result = chatConversationPage.IOgetline();
        switch (result)
        {
        case chat::send_message:
            sendMessage(chat::private_chat);
            break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::public_chat:
            result = chat::public_chat;
            break;
        case chat::back:
            result = chat::empty;
            break;
        default:
            break;
        }
        system(clear);
    } while ((result != chat::public_chat) && (result != chat::empty));
    pgDefault();
    pm_user = nullptr;
    return result;
}
