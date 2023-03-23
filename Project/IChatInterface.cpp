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

/*
Алгоритм пагинатора:

*start end - начинают отсчет с нуля.
*если массив пустой - start == end == 0. В этом случае условие в цикле i=start; i<end не сработает
*если в массиве один элемент maxMsgs == 1 - start = 0 end = 1, тогда условие цикла сработает 1 раз.
*если в массиве 2 элемента maxMsgs == 2. start = 0 end = 2. end = start + msgPerPage
*Вывод: если массив не пустой, end = start + msgPerPage
*если пустой start == 0 && end == 0 - выход


*/
void IChatInterface::pagination(int maxMsgs, int msgPerPage, int pageNumber, int *start, int *end, int *maxPageNumber)
{
    // Если пустой массив
    if (maxMsgs == 0)
    {
        *start = 0;
        *end = 0;
        return;
    }

    // количество сообщений на страницу не должно превышать максимального количества сообщений
    if (msgPerPage > maxMsgs)
    {
        msgPerPage = maxMsgs;
    }
    // максимально возможный номер страницы, урезается если введен превышающий диапазон
    *maxPageNumber = (maxMsgs / msgPerPage) + 1;
    if (pageNumber > *maxPageNumber)
    {
        pageNumber = *maxPageNumber;
    }

    // первое сообщение страницы
    *start = msgPerPage * (pageNumber - 1);
    // конечное сообщение в странице
    *end = *start + msgPerPage;

    // если msgPerPage превысил максимум
    if (*end > maxMsgs)
    {
        *start = maxMsgs - msgPerPage;
        *end = maxMsgs;
    }
}