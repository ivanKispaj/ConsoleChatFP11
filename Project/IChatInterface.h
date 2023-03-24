#pragma once
#include "DB.h"
#include <iostream>
#include <memory>
#include "UserInput.h"
const extern char clear[];

/**
 * Чат имеет ветвистую структуру, пользователь будет постоянно перемещаться по различным диалогам.
 * Каждый диалог может иметь различные результаты.
 * Например регистрация или логин могут успешными и неуспешными.
 * Каждый метод работающий с вводом пользователя будет возвращать элементы enum типа,
 * соответствующие результату обработки ввода.
 */

/// @brief Возвращаемые результаты работы диалога с пользователем.
enum Results
{
    login_success,
    login_cancel,
    register_success,
    register_cancel,
    yes,
    no,
    app_exit,
    back,
    empty,
    chat,
    admin,
    registration,
    login,
    send_message,
    chat_options,
};

class IChatInterface
{
protected:
    std::unique_ptr<User> user = nullptr;

public:
    std::unique_ptr<DB> db;
    IChatInterface() = default;
    virtual ~IChatInterface() = default;
    virtual Results run(std::unique_ptr<DB> _db) = 0;

    /// @brief Проверяет логин и пароль при входе пользователя.
    /// @return
    Results login();

    /// @brief Пагинатор. Высчитывает начальный и конечный индексы диапазона элементов в массиве
    /// @param maxMsgs Максимальное количество элементов
    /// @param msgPerPage Количество элементов в странице
    /// @param pageNumber Номер страницы
    /// @param start Для сохранения первого элемента страницы
    /// @param end Для сохранения второго элемента страницы
    void pagination(int maxMsgs, int msgPerPage, int pageNumber, int *start, int *end, int *maxPageNumber);
};