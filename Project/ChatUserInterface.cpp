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
        userInput = chatAreaPage.IOAction();
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

    std::cout << "Вы находитесь в меню регистрации" << std::endl;
    UserInput<std::string, std::string> getLogin("Страница регистрации", "Введите логин: ");
    UserInput<std::string, std::string> getName(std::string(), "Введите отображаемое имя пользователя: ");
    UserInput<std::string, std::string> getPass(std::string(), "Введите пароль: ");
    UserInput<std::string, Results> regEnd(std::string(), "Выберите действие (з - Зарегистрироваться, о - Отменить регистрацию): ", "Неверный ввод", 2);
    regEnd.addInputs("з", "о");
    regEnd.addOutputs(Results::register_success, Results::register_cancel);

    // логин
    bool validLogin = false;
    do
    {
        login = getLogin.throughIOAction();

        validLogin = db->isUniqueLogin(login);
        if (!validLogin)
        {
            std::cout << "Этот логин занят!" << std::endl;
        }
    } while (!validLogin);

    password = getPass.throughIOAction();
    name = getName.throughIOAction();

    Results endInput;

    endInput = regEnd.IOAction();
    if (endInput == Results::register_cancel)
    {
        return Results::register_cancel;
    }
    User user(name, login, password);
    db->addUser(user);
    return register_success;
}

Results ChatUserInterface::chat()
{
    std::cout << "Здравствуйте, " << user->getUserName() << "!" << std::endl;
    std::cout << "Вы успешно вошли в чат." << std::endl;
    int pageNumber = 0;
    int msgPerPages = 0;
    int msgMaxCount = 0;
    int start = 0;
    int end = 0;

    std::string chatDescription = user->getUserName() + " [" + user->getUserLogin() +
                                  "] Общий чат. Сообщения: " +
                                  std::to_string(start) + " - " +
                                  std::to_string(end) + " из " +
                                  std::to_string(msgMaxCount);
    std::string mainMessage = "Выберите действие:\n"
                              "с - написать сообщение;\n"
                              "н - настройки;";

    db->getAllMessage();

    UserInput<std::string, Results> chatMainPage(chatDescription, mainMessage, "Неверный ввод", 2);
    chatMainPage.addInputs("з", "о");
    chatMainPage.addOutputs(Results::register_success, Results::register_cancel);

    do
    {
        chatMainPage.IOAction();
    } while (1);
    return empty;
}