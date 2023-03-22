#include "IChatInterface.h"

Results IChatInterface::login()
{
    std::string login;
    std::string password;
    char endInput = 'y';
    bool validate = false;

    // ввод логина
    do
    {
        login = getInput<std::string>("Введите логин: ");

        try
        {
            std::unique_ptr<User> _user = db->getUserByLogin(login, true);
            user = std::move(_user);
            validate = true;
        }
        catch (UserNotFoundException &e)
        {
            std::cout << e.what() << std::endl;

            endInput = getInput<char>("Отменить вход? (y - да, n - нет): ");
            if (endInput == 'y')
            {
                return login_cancel;
            }
            continue;
        }
    } while (!validate);

    // ввод пароля
    validate = false;
    do
    {
        password = getInput<std::string>("Введите пароль: ");
        validate = db->isCorrectPassword(user->getId(), password);

        if (!validate)
        {
            std::cout << "Неверный пароль: " << std::endl;
            endInput = getInput<char>("Отменить вход? (y - да, n - нет): ");
            if (endInput == 'y')
            {
                return login_cancel;
            }
            continue;
        }
    } while (!validate);
    return login_success;
}

void IChatInterface::pagination(int *maxMsgs, int *msgPerPage, int *pageNumber, int *start, int *end)
{
    // количество сообщений на страницу не должно превышать максимального количества сообщений
    if (*msgPerPage > *maxMsgs)
    {
        msgPerPage = maxMsgs;
    }
    // максимально возможный номер страницы, урезается если введен превышающий диапазон
    int maxPageNumber = (*maxMsgs / *msgPerPage) + 1;
    if (*pageNumber > maxPageNumber)
    {
        *pageNumber = maxPageNumber;
    }
    // первое сообщение страницы
    *start = *msgPerPage * (*pageNumber - 1);
    // конечное сообщение в странице
    *end = *start + *msgPerPage;
    if (*end > *maxMsgs)
    {
        *end = *maxMsgs;
    }
}