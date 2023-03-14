#include "ChatUserInterface.h"
ChatUserInterface::ChatUserInterface(DB *_db) : IChatInterface(_db) {}

Results ChatUserInterface::run()
{
    char userInput = 'l';
    Results result = empty;
    do
    {
        // будем оставаться в этом меню после регистрации и логина
        userInput = getInput<char>("Выберите действия (r - регистрация, l - войти, s - в начало, e - выход из программы): ");
        switch (userInput)
        {
        case 'r':
            result = registration();
            break;
        case 'l':
            result = loginInChat();
            break;
        case 'e':
            result = app_exit;
            break;
        case 'b':
            result = back;
            break;
        default:
            std::cout << "Неверный ввод" << std::endl;
            result = empty;
            break;
        }
    } while (result != app_exit && result != back);
    return result;
}

Results ChatUserInterface::loginInChat()
{
    auto result = login();
    if (result == login_success)
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

    bool validLogin = false;
    do
    {
        login = getInput<std::string>("Укажите уникальный логин (он будет использоваться для входа): ");
        validLogin = db->isUniqueLogin(login);
        if (!validLogin)
        {
            std::cout << "Этот логин занят!" << std::endl;
        }
    } while (!validLogin);

    password = getInput<std::string>("Укажите пароль: ");
    name = getInput<std::string>("Укажите отображаемое имя пользователя: ");

    char endInput = 'y';

    endInput = getInput<char>("Завершить регистрацию? (y - да, n - отменить): ");
    if (endInput == 'n')
    {
        return register_cancel;
    }
    User user(name, login, password);
    db->addUser(user);
    return register_success;
}

Results ChatUserInterface::chat()
{
    std::cout << "Вы успешно вошли в чат." << std::endl;
    std::cout << "Ваше имя: " << user->getUserName() << std::endl;
    std::cout << "Ваш логин: " << user->getUserLogin() << std::endl;
    std::cout << "Чат в разработке. До свидания." << std::endl;
    return empty;
}