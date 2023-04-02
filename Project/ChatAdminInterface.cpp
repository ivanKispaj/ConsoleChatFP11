#include "ChatAdminInterface.h"

chat::Results ChatAdminInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    auto complaintBot = db->getUserByLogin("complaint_bot");
    this->complaintBot = std::move(complaintBot);
    auto _login = login();
    if (_login == chat::login_success && user->isAdmin())
    {
        mainPage();
    }
    else
    {
        std::cout << "У вас не получилось войти в раздел администратора." << std::endl;
        return chat::login_cancel;
    }
    return chat::empty;
}

void ChatAdminInterface::mainPage()
{
    UserInput<std::string, chat::Results> adminAreaPage(
        "Раздел администратора.",
        "Опции: "
        "\nп - управление пользователями;"
        "\nс - управление сообщениями;"
        "\nж - рассмотрение жалоб;"
        "\nв - выход;"
        "\nУкажите опцию: ",
        "Неверный ввод",
        4);

    adminAreaPage.addInputs("п", "с", "ж", "в");
    adminAreaPage.addOutputs(chat::user_list, chat::messages, chat::complaint, chat::back);

    auto result = chat::empty;
    do
    {
        switch (result)
        {
        case chat::user_list:
            complaintManage();
            break;
        case chat::messages:
            messagesManage();
            break;
        case chat::complaint:
            usersManage();
            break;
        case chat::back:
            return;
            break;
        default:
            break;
        }
    } while (1);
}

void ChatAdminInterface::complaintManage()
{
    system(clear);
    pgDefault();
    UserInput<std::string, chat::Results> complaintPage("Список жалоб",
                                                        "Опции: "
                                                        "\nу - удаление жалобы;"
                                                        "\nп - управление пользователями;"
                                                        "\nс - управление сообщениями;"
                                                        "\nн - назад;"
                                                        "\nУкажите опцию: ",
                                                        "Неверный ввод", 4);
    complaintPage.addInputs("у", "п", "с", "н");
    complaintPage.addOutputs(chat::delete_message,
                             chat::user_list,
                             chat::messages,
                             chat::back);
    chat::Results result = chat::empty;
    std::string chatDescription;
    do
    {
        switch (result)
        {
        case chat::delete_message:
            /* code */
            break;

        default:
            break;
        }
    } while (1);
}

void ChatAdminInterface::messagesManage()
{
}

void ChatAdminInterface::usersManage()
{
}

void ChatAdminInterface::deleteMessage()
{
}

void ChatAdminInterface::userBan()
{
}