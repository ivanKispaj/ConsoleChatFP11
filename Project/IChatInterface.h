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
    DB *db;
    std::unique_ptr<User> user = nullptr;

public:
    IChatInterface() = default;
    IChatInterface(DB *_db)
    {
        db = _db;
    }
    virtual ~IChatInterface() = default;
    virtual Results run() = 0;
    Results login();
};