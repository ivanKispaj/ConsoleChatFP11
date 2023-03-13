#include "IChatInterface.h"

int IChatInterface::login()
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
    return 1;
}