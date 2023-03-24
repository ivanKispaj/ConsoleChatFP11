#include "IChatInterface.h"

Results IChatInterface::login()
{
    std::string _login;    
    Results endInput = Results::empty;
    std::string incorrectInput = "Неверный ввод. Пустые значения недопустимы.";

    UserInput<std::string, std::string> getLogin("Страница входа", "Введите логин: ", incorrectInput);
    UserInput<std::string, std::string> getPass(std::string(), "Введите пароль: ", incorrectInput);
    UserInput<std::string, Results> loginCancel(std::string(), "Отменить вход? (да - отменить, нет - продолжить): ", "Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs("да", "нет", "yes", "no");
    loginCancel.addOutputs(Results::yes, Results::no, Results::yes, Results::no);

    // ввод логина
    do
    {
        _login = getLogin.IOgetlineThrough(true);

        try
        {
            std::unique_ptr<User> _user = db->getUserByLogin(_login, true);
            user = std::move(_user);
            break;
        }
        catch (UserNotFoundException &e)
        {
            std::cout << e.what() << std::endl;
            endInput = loginCancel.IOgetline();
            if (endInput == Results::yes)
            {
                return login_cancel;
            }
        }
    } while (1);

    // ввод пароля
    bool validate = false;
    std::string password;
    do
    {
        password = getPass.IOgetlineThrough(true);
        validate = db->isCorrectPassword(user->getId(), password);
        if (!validate)
        {
            std::cout << "Неверный пароль: " << std::endl;
            endInput = loginCancel.IOgetline();
            if (endInput == Results::yes)
            {
                return login_cancel;
            }
        }
    } while (!validate);
    return login_success;
}

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