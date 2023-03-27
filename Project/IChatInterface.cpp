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

void IChatInterface::pagination()
{
    if (msgStart < 0)
    {
        msgStart = 0;
    }
    if (pageNumber < 1)
    {
        pageNumber = 1;
    }
    // Если пустой массив
    if (msgMaxCount == 0)
    {
        msgStart = 0;
        msgEnd = 0;
        return;
    }

    // количество сообщений на страницу не должно превышать максимального количества сообщений
    if (msgPerPage >= msgMaxCount)
    {
        msgStart = 0;
        msgEnd = msgMaxCount;
        return;
    }

    if (msgPerPage < 1)
    {
        msgPerPage = 1;
    }

    // максимально возможный номер страницы, урезается если введен превышающий диапазон
    maxPageNumber = (msgMaxCount / msgPerPage) + 1;
    if (pageNumber > maxPageNumber)
    {
        pageNumber = maxPageNumber;
    }

    // если запрошена страница
    if (paginationMode == PaginationMode::page)
    {
        msgStart = msgPerPage * (pageNumber - 1);
        msgEnd = msgStart + msgPerPage;
    }

    // если запрошен показ страницы от конкретного сообщения
    if (paginationMode == PaginationMode::message)
    {
        msgEnd = msgStart + msgPerPage;
    }

    // msgStart + msgPerPage превысил максимум или запрошена последняя страница
    // будут отображаться последние msgPerPage сообщений
    if ((msgEnd > msgMaxCount) || (paginationMode == PaginationMode::last_page))
    {
        msgStart = msgMaxCount - msgPerPage;
        msgEnd = msgMaxCount;
    }
}

std::string IChatInterface::StampToTime(long long timestamp)
{
    time_t tick = (time_t)(timestamp);
	struct tm tm;
	char s[40];
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%d.%m.%Y %H:%M:%S", &tm);
	std::string str(s);
	return str;
}
