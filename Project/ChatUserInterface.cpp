#include "ChatUserInterface.h"
ChatUserInterface::ChatUserInterface(DB *_db) : IChatInterface(_db) {}

Input ChatUserInterface::run()
{
    char userInput = 'l';

    do
    {
        userInput = getInput<char>("Выберите действия (r - регистрация, l - войти, b - в начало, e - выход из программы): ");
        switch (userInput)
        {
        case 'r':
            return registration();
            break;
        case 'l':
            return login();
        case 'e':
            return close;
        case 'b':
            return back;
        default:
            break;
        }
    } while (1);
    return ok;
}

Input ChatUserInterface::registration()
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
    } while (!validLogin);

    password = getInput<std::string>("Укажите пароль: ");
    name = getInput<std::string>("Укажите отображаемое имя пользователя: ");

    char endInput = 'y';

    endInput = getInput<char>("Завершить регистрацию? (y - да, n - отменить): ");
    if (endInput == 'n')
    {
        return cancel;
    }
    User user(name, login, password);
    db->addUser(user);
    return ok;
}

Input ChatUserInterface::chat()
{
    std::cout << "Вы успешно вошли в чат." << std::endl;
    std::cout << "Ваше имя: " << user.getUserName() << std::endl;
    std::cout << "Ваш логин: " << user.getUserLogin() << std::endl;
    std::cout << "Чат в разработке. До свидания." << std::endl;
    return ok;
}