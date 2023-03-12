#include "ChatUserInterface.h"
ChatUserInterface::ChatUserInterface(DB *_db) : IChatInterface(_db) {}

int ChatUserInterface::run()
{
    char userInput = 'l';
    int exitCode = 1;
    do
    {
        std::cout << "Выберите действия (r - регистрация, l - войти, b - в начало, e - выход из программы): ";
        std::cin >> userInput;
        switch (userInput)
        {
        case 'r':
            exitCode = registration();
            break;
        case 'l':
            exitCode = login();
            break;
        case 'e':
            return 0;
            break;
        case 'b':
            return 1;
            break;
        default:
            break;
        }
    } while (exitCode);

    return exitCode;
}

int ChatUserInterface::registration()
{
    std::string login;
    std::string name;
    std::string password;

    std::cout << "Вы находитесь в меню регистрации" << std::endl;

    bool cond = false;
    do
    {
        std::cout << "Укажите уникальный логин (он будет использоваться для входа): ";
        std::cin >> login;
        cond = db->isUniqueLogin(login);
    } while (!cond);
    std::cout << "Укажите пароль: ";
    std::cin >> password;

    std::cout << "Укажите отображаемое имя пользователя: ";
    std::cin >> name;

    char endInput = 'y';

    std::cout << "Завершить регистрацию? (y - да, n - отменить): ";
    std::cin >> endInput;
    if (endInput == 'n')
    {
        return 1;
    }
    User user(name, login, password);
    db->addUser(user);
    return 1;
}

int ChatUserInterface::login()
{
    std::string login;
    std::string password;
    char endInput = 'y';
    bool condition = true;
    do
    {
        std::cout << "Введите логин: ";
        std::cin >> login;

        std::cout << "Введите пароль: ";
        std::cin >> password;
        user = *db->getUserByLogin(login);
        if (&user == nullptr)
        {
            std::cout << "Неверный логин: " << std::endl;
            condition = false;
            std::cout << "Отменить вход? (y - да, n - нет): ";
            std::cin >> endInput;
            if (endInput == 'y')
            {
                return 1;
            }
            continue;
        }
        if (!db->isCorrectPassword(user.getUserId(), password))
        {
            std::cout << "Неверный пароль: " << std::endl;
            condition = false;
            std::cout << "Отменить вход? (y - да, n - нет): ";
            std::cin >> endInput;
            if (endInput == 'y')
            {
                return 1;
            }
            continue;
        }
    } while (!condition);

    return chat();
}

int ChatUserInterface::chat()
{
    std::cout << "Вы успешно вошли в чат." << std::endl;
    std::cout << "Ваше имя: " << user.getUserName() << std::endl;
    std::cout << "Ваш логин: " << user.getUserLogin() << std::endl;
    std::cout << "Чат в разработке. До свидания." << std::endl;
    return 1;
}