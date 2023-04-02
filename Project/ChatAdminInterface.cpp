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
        result = adminAreaPage.IOgetline();
        switch (result)
        {
        case chat::user_list:
            usersManage();
            break;
        case chat::messages:
            messagesManage();
            break;
        case chat::complaint:
            complaintManage();
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
                                                        "\nнс - навигация по списку жалоб;"
                                                        "\nп - управление пользователями;"
                                                        "\nс - управление сообщениями;"
                                                        "\nн - назад;"
                                                        "\nУкажите опцию: ",
                                                        "Неверный ввод", 5);
    complaintPage.addInputs("у", "нс", "п", "с", "н");
    complaintPage.addOutputs(chat::delete_message,
                             chat::chat_options,
                             chat::user_list,
                             chat::messages,
                             chat::back);
    chat::Results result = chat::empty;
    std::string chatDescription;
    do
    {
        system(clear);
        auto complaintList = db->getAllPrivateMessagesForUserById(complaintBot->getId(), pg_MaxItems);

        messagesList(std::move(complaintList));

        if (pg_MaxItems <= 0)
        {
            std::cout << "Жалобы отсутствуют." << std::endl;
        }
        chatDescription = "Список жалоб. Показаны жалобы: " + pgInfo();
        complaintPage.setDescription(chatDescription);
        result = complaintPage.IOgetline();
        switch (result)
        {
        case chat::delete_message:
        {
            if (pg_MaxItems > 0)
            {
                deleteMessage(true);
            }
        }
        break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::user_list:
            usersManage();
            break;
        case chat::messages:
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

void ChatAdminInterface::messagesManage()
{
}

void ChatAdminInterface::usersManage()
{
}

void ChatAdminInterface::deleteMessage(bool complaint_only)
{
    UserInput<int, int> getMessage(std::string(), "Укажите messageId: ", std::string());
    UserInput<std::string, chat::Results> yesnoIO(std::string(), "Вы действительно хотите удалить сообщение? (да - удалить / нет - не удалять): ", "Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs("да", "нет", "yes", "no");
    yesnoIO.addOutputs(chat::yes, chat::no, chat::yes, chat::no);
    int msgId = 0;
    do
    {
        int msgId = getMessage.IOcinThrough();
        auto message = db->getMessage(msgId);
        if (message != nullptr)
        {
            if (complaint_only && (message->getRecipientID() != 2))
            {
                std::cout << "Сообщение не является жалобой." << std::endl;
                continue;
            }
            auto result = yesnoIO.IOgetline();
            if (result == chat::yes)
            {
                db->deleteMessageById(msgId);
                return;
            }
            else
            {
                return;
            }
        }
        std::cout << "Указан неверный messageId" << std::endl;
    } while (true);
}

void ChatAdminInterface::userBan()
{
}