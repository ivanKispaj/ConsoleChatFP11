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
    if (pg_msgStart < 0)
    {
        pg_msgStart = 0;
    }
    if (pg_pageNumber < 1)
    {
        pg_pageNumber = 1;
    }
    // Если пустой массив
    if (pg_msgMaxCount == 0)
    {
        pg_msgStart = 0;
        pg_msgEnd = 0;
        return;
    }

    // количество сообщений на страницу не должно превышать максимального количества сообщений
    if (pg_msgPerPage >= pg_msgMaxCount)
    {
        pg_msgStart = 0;
        pg_msgEnd = pg_msgMaxCount;
        return;
    }

    if (pg_msgPerPage < 1)
    {
        pg_msgPerPage = 1;
    }

    // максимально возможный номер страницы, урезается если введен превышающий диапазон
    pg_maxPageNumber = (pg_msgMaxCount / pg_msgPerPage) + 1;
    if (pg_pageNumber > pg_maxPageNumber)
    {
        pg_pageNumber = pg_maxPageNumber;
    }

    // если запрошена страница
    if (paginationMode == PaginationMode::page)
    {
        pg_msgStart = pg_msgPerPage * (pg_pageNumber - 1);
        pg_msgEnd = pg_msgStart + pg_msgPerPage;
    }

    // если запрошен показ страницы от конкретного сообщения
    if (paginationMode == PaginationMode::message)
    {
        pg_msgEnd = pg_msgStart + pg_msgPerPage;
    }

    // pg_msgStart + pg_msgPerPage превысил максимум или запрошена последняя страница
    // будут отображаться последние pg_msgPerPage сообщений
    if ((pg_msgEnd > pg_msgMaxCount) || (paginationMode == PaginationMode::last_page))
    {
        pg_msgStart = pg_msgMaxCount - pg_msgPerPage;
        pg_msgEnd = pg_msgMaxCount;
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

void IChatInterface::defaultOptions()
{
    paginationMode = PaginationMode::last_page;
    pg_pageNumber = 1;
    pg_msgMaxCount = 0;
    pg_msgPerPage = 10;
    pg_maxPageNumber = 0;
    pg_msgStart = 0;
    pg_msgEnd = 0;
}

void IChatInterface::chatNavigation()
{
    UserInput<std::string, PaginationMode> selectOption(std::string(),
                                                        "Выберите опцию:"
                                                        "\nснс - сообщений на странице;"
                                                        "\nпнс - перейти на страницу...;"
                                                        "\nпкс - перейти к сообщению №...;"
                                                        "\nсбр - сброс настроек (всегда последние 10 сообщений);"
                                                        "\nн - вернуться в чат;"
                                                        "\nВведите значение: ",
                                                        "Неверный ввод.", 5);
    selectOption.addInputs("снс", "пнс", "пкс", "сбр", "н");
    selectOption.addOutputs(PaginationMode::msg_per_page, PaginationMode::page, PaginationMode::message, PaginationMode::last_page, PaginationMode::close_options);

    UserInput<int, int> getInt(std::string(), std::string(), "Неверный ввод");

    switch (selectOption.IOgetline())
    {
    case PaginationMode::msg_per_page:
        getInt.setMainMessage("Укажите количество сообщений на странице (1 - " + std::to_string(pg_msgMaxCount) + "): ");
        pg_msgPerPage = getInt.IOcinThrough();
        break;
    case PaginationMode::page:
        getInt.setMainMessage("Укажите номер страницы (1 - " + std::to_string(pg_maxPageNumber - 1) + "): ");
        pg_pageNumber = getInt.IOcinThrough();
        paginationMode = PaginationMode::page;
        break;
    case PaginationMode::message:
        getInt.setMainMessage("Укажите номер сообщения (1 - " + std::to_string(pg_msgMaxCount) + "): ");
        pg_msgStart = getInt.IOcinThrough() - 1;
        paginationMode = PaginationMode::message;
        break;
    case PaginationMode::last_page:
        pg_msgMaxCount = 10;
        paginationMode = PaginationMode::last_page;
    default:
        break;
    }
}
