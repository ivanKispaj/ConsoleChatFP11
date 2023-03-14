#include "IChatInterface.h"

Input IChatInterface::login()
{
    std::string login;
    std::string password;
    char endInput = 'y';
    bool validate = false;
    do
    {
        login = getInput<std::string>("Введите логин: ");
        password = getInput<std::string>("Введите пароль: ");
        std::unique_ptr<User> user1 = db->getUserByLogin(login);
        user = std::move(user1);
        validate = (user != nullptr) && db->isCorrectPassword(user->getUserId(), password);

        if (!validate)
        {
            std::cout << "Неверный логин или пароль: " << std::endl;
            endInput = getInput<char>("Отменить вход? (y - да, n - нет): ");
            if (endInput == 'y')
            {
                return cancel;
            }
            continue;
        }
    } while (!validate);
    return ok;
}