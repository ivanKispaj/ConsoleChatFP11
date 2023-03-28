#pragma once
#include "DB.h"
#include <iostream>
#include <memory>
#include "UserInput.h"
#include "EnumOptions.h"

const extern char clear[];


class IChatInterface
{
protected:
    std::unique_ptr<User> user = nullptr;

    // настройки пагинации
    page::PaginationMode paginationMode = page::last_page;
    int pg_pageNumber = 1;
    int pg_msgMaxCount = 0;
    int pg_msgPerPage = 10;
    int pg_maxPageNumber = 0;
    int pg_msgStart = 0;
    int pg_msgEnd = 0;

public:
    std::unique_ptr<DB> db;
    IChatInterface() = default;
    virtual ~IChatInterface() = default;
    virtual chat::Results run(std::unique_ptr<DB> _db) = 0;

    /// @brief Проверяет логин и пароль при входе пользователя.
    /// @return
    chat::Results login();

    /// @brief Пагинатор. Высчитывает начальный и конечный индексы диапазона в массиве сообщений.
    void pagination();

    /// @brief Преобразует timestamp в дату/время
    /// @param timestamp
    /// @return
    std::string StampToTime(long long timestamp);

    void defaultOptions();

    void chatNavigation();
};