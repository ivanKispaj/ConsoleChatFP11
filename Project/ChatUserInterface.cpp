#include "ChatUserInterface.h"

Results ChatUserInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    Results userInput = Results::empty;
    Results result = Results::empty;
    UserInput<std::string, Results> chatAreaPage("Страница авторизации и регистрации",
                                                 "Выберите действие (р - Регистрация, вх - Вход, н - Назад, вых - выход): ",
                                                 "Неверный ввод", 4);
    chatAreaPage.addInputs("р", "вх", "н", "вых");
    chatAreaPage.addOutputs(Results::registration, Results::login, Results::back, Results::app_exit);
    do
    {
        system(clear);
        userInput = chatAreaPage.IOgetline();
        switch (userInput)
        {
        case Results::registration:
            result = registration();
            break;
        case Results::login:
            result = loginInChat();
            break;
        case Results::back:
            result = Results::back;
            break;
        case Results::app_exit:
            result = Results::app_exit;
            break;
        default:
            break;
        }
    } while (result != Results::app_exit && result != Results::back);
    return result;
}

Results ChatUserInterface::loginInChat()
{
    auto result = login();
    if (result == Results::login_success)
    {
        return chat();
    }
    return result;
}

Results ChatUserInterface::registration()
{
    std::string login;
    std::string name;
    std::string password;

    std::string incorrectInput = "Неверный ввод. Пустые значения недопустимы.";
    std::cout << "Вы находитесь в меню регистрации" << std::endl;
    UserInput<std::string, std::string> getLogin("Страница регистрации", "Введите логин: ", incorrectInput);
    UserInput<std::string, std::string> getName(std::string(), "Введите отображаемое имя пользователя: ", incorrectInput);
    UserInput<std::string, std::string> getPass(std::string(), "Введите пароль: ", incorrectInput);
    UserInput<std::string, Results> regEnd(std::string(), "Выберите действие (з - Зарегистрироваться, о - Отменить регистрацию): ", "Неверный ввод", 2);
    regEnd.addInputs("з", "о");
    regEnd.addOutputs(Results::register_success, Results::register_cancel);

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
    Results endInput;
    endInput = regEnd.IOgetline();
    if (endInput == Results::register_cancel)
    {
        return Results::register_cancel;
    }

    // При удачном завершении регистрации - переход в чат
    User _user(name, login, password);
    db->addUser(_user);
    user = std::make_unique<User>(_user);
    chat();
    return register_success;
}

Results ChatUserInterface::chat()
{
    // system(clear);
    std::cout << std::endl;
    std::cout << "Здравствуйте, " << user->getUserName() << "!" << std::endl;
    std::cout << "Вы успешно вошли в чат." << std::endl;
    std::cout << std::endl;
    std::string chatDescription;
    std::string mainMessage;

    mainMessage = "Выберите действие: "
                  "(с - написать сообщение; "
                  "н - настройки; "
                  "в - выход): ";

    UserInput<std::string, Results> chatMainPage(chatDescription, mainMessage, "Неверный ввод", 3);
    chatMainPage.addInputs("с", "н", "в");
    chatMainPage.addOutputs(Results::send_message, Results::chat_options, Results::back);

    Results result;

    do
    {
        auto messages = db->getAllPublicMessages(msgMaxCount);

        pagination();

        if (messages == nullptr)
        {
            std::cout << "В этом чате нет сообщений. Начните общение первым." << std::endl;
        }

        for (int i{msgStart}; i < msgEnd && messages != nullptr; i++)
        {
            auto msgUser = db->getUserById(messages[i].getAuthorID());

            std::cout << std::endl;
            char time[128];
            time_t tick = (time_t)(messages[i].getDate()); // conversion time
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
                          std::to_string((messages == nullptr) ? msgStart : msgStart + 1) + " - " +
                          std::to_string(msgEnd) + " из " +
                          std::to_string(msgMaxCount);
        chatMainPage.setDescription(chatDescription);

        result = chatMainPage.IOgetline();
        switch (result)
        {
        case Results::send_message:
            sendMessage();
            break;
        case Results::chat_options:
            chatNavigation();
            break;
        case Results::back:
            paginationMode = PaginationMode::last_page;
            pageNumber = 1;
            msgMaxCount = 0;
            msgPerPage = 10;
            maxPageNumber = 0;
            msgStart = 0;
            msgEnd = 0;
            user = nullptr;
            break;
        default:
            break;
        }
        system(clear);
    } while (result != Results::back);
    return Results::empty;
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

void ChatUserInterface::chatNavigation()
{
    UserInput<std::string, PaginationMode> selectOption(std::string(),
                                                        "Выберите опцию:"
                                                        "\nснс - сообщений на странице;"
                                                        "\nпнс - перейти на страницу...;"
                                                        "\nпкс - перейти к сообщению №...;"
                                                        "\nсбр - сброс настроек (всегда последние 10 сообщений);"
                                                        "\nн - вернуться в чат;"
                                                        "\nВведите значение: ",
                                                        "Неверный ввод.", 5);
    selectOption.addInputs("снс", "пнс", "пкс", "сбр", "н");
    selectOption.addOutputs(PaginationMode::msg_per_page, PaginationMode::page, PaginationMode::message, PaginationMode::last_page, PaginationMode::close_options);

    UserInput<int, int> getInt(std::string(), std::string(), "Неверный ввод");

    switch (selectOption.IOgetline())
    {
    case PaginationMode::msg_per_page:
        getInt.setMainMessage("Укажите количество сообщений на странице (1 - " + std::to_string(msgMaxCount) + "): ");
        msgPerPage = getInt.IOcinThrough();
        break;
    case PaginationMode::page:
        getInt.setMainMessage("Укажите номер страницы (1 - " + std::to_string(maxPageNumber - 1) + "): ");
        pageNumber = getInt.IOcinThrough();
        paginationMode = PaginationMode::page;
        break;
    case PaginationMode::message:
        getInt.setMainMessage("Укажите номер сообщения (1 - " + std::to_string(msgMaxCount) + "): ");
        msgStart = getInt.IOcinThrough() - 1;
        paginationMode = PaginationMode::message;
        break;
    case PaginationMode::last_page:
        msgMaxCount = 10;
        paginationMode = PaginationMode::last_page;
    default:
        break;
    }
}