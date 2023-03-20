#pragma once
#include "DB.h"
#include "Misc.h"
#include <iostream>
#include <memory>

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
    empty
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
    Results login();
};