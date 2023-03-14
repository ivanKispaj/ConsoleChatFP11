#pragma once
#include "DB.h"
#include "Misc.h"
#include <iostream>
#include <memory>

enum Input
{
    admin,
    chat,
    close,
    back,
    start,
    yes,
    no,
    ok,
    wrong,
    empty,
    cancel
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
    virtual Input run() = 0;
    Input login();
};