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
        pg_Default();
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

        pagination();

        if (messages == nullptr)
        {
            std::cout << "В этом чате нет сообщений. Начните общение первым." << std::endl;
        }

        for (int i{pg_StartItem}; i < pg_EndItem && messages != nullptr; i++)
        {
            auto msgUser = db->getUserById(messages[i].getAuthorID());

            std::cout << std::endl;
            std::cout
                << i + 1 << ". "
                << StampToTime(messages[i].getDate()) + " "
                << msgUser->getUserName()
                << "[" << msgUser->getUserLogin() << "] "
                << "\t[messageID " << messages[i].getId() << "] "
                << "[userID " << std::to_string(msgUser->getId()) << "]"
                << std::endl;
            std::cout << messages[i].getMessage() << std::endl;
        }
        std::cout << std::endl;
        chatDescription = user->getUserName() + " [" + user->getUserLogin() +
                          "] Общий чат. Показаны сообщения: " +
                          std::to_string((messages == nullptr) ? pg_StartItem : pg_StartItem + 1) + " - " +
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
            chatNavigation();
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

void ChatUserInterface::sendMessage()
{
    Message message;
    std::string messageText;
    UserInput<std::string, std::string> send(std::string(), "Введите текст сообщения: ", "Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);

    message.setAuthorID(user->getId());
    message.setMessage(messageText);

    db->AddMessageToAllUsers(message);
}

chat::Results ChatUserInterface::privateChat()
{
    system(clear);

    UserInput<std::string, chat::Results> privateMainPage("Личные сообщения. Главная страница.",
                                                          "Выбор пользователя:\n"
                                                          "\tл - по логину;\n"
                                                          "\tи - по userID;\n"
                                                          "Другие опции:\n"
                                                          "\tн - навигация по списку пользователей;"
                                                          "\tо - вернуться в общий чат;"
                                                          "\tв - выйти из чата"
                                                          "Укажите опцию: ",
                                                          "Неверный ввод", 5);
    privateMainPage.addInputs("л", "и", "н", "о", "в");
    privateMainPage.addOutputs(chat::search_user_byLogin,
                               chat::search_user_byId,
                               chat::user_list,
                               chat::public_chat,
                               chat::back);
    chat::Results result = chat::public_chat;
    std::string chatDescription;
    do
    {
        auto users = db->getAllUsers();
        pg_MaxItems = db->usersCount();
        pagination();

        if (users == nullptr)
        {
            std::cout << "В этом чате нет пользователей. Пригласите других участников." << std::endl;
        }
        for (int i{pg_StartItem}; i < pg_EndItem && users != nullptr; i++)
        {
            std::cout
                << i + 1 << ". "

                << users[i].getUserName()
                << "[" << users[i].getUserLogin() << "] "
                << "\t[userID " << std::to_string(users[i].getId()) << "]"
                << std::endl;
            std::cout << std::endl;
        }
        std::cout << std::endl;
        chatDescription = user->getUserName() + " [" + user->getUserLogin() +
                          "] Личные сообщения. Главная страница. Показаны пользователи: " +
                          std::to_string((users == nullptr) ? pg_StartItem : pg_StartItem + 1) + " - " +
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
        default:
            break;
        }
    } while (1);

    return result;
}

void ChatUserInterface::searchUser(chat::Results results)
{
    UserInput<int, int> searchByIdInput("Писк пользователя по userID", "Введите userID: ", "Неверный ввод");
    UserInput<std::string, std::string> searchByNameInput("Писк пользователя по логину", "Введите логин пользователя: ", "Неверный ввод");
    if (results = chat::search_user_byId)
    {
        int uid = searchByIdInput.IOcinThrough();
        std::unique_ptr<User> _user = db->getUserById(uid);
        pm_user = std::move(_user);
        if (pm_user == nullptr)
        {
            std::cout << "Пользователь с userId = " + std::to_string(uid) + " не существует." << std::endl;
        }
    }
    if (results == chat::search_user_byLogin)
    {
        std::string login = searchByIdInput.IOgetlineThrough();
        std::unique_ptr<User> _user = db->getUserByLogin(login);
        pm_user = std::move(_user);
        if (pm_user == nullptr)
        {
            std::cout << "Пользователь с логином = " + login + " не существует." << std::endl;
        }
    }
}

chat::Results ChatUserInterface::privateChatWithUser(chat::Results result)
{
    searchUser(result);
    if (pm_user == nullptr)
    {
        return chat::user_not_found;
    }
    return chat::Results();
}
