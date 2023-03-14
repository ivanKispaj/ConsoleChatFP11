#pragma once
#include "DB.h"
#include "Misc.h"
#include <iostream>
#include <memory>

enum Input
{
    admin = 'a',
    chat = 'c',
    close = 'e',
    back = 'b',
    start = 's',
    yes = 'y',
    no = 'n',
    ok,
    wrong,
    empty,
    cancel
};
class IChatInterface
{
protected:
    DB *db;
    std::unique_ptr<User> user;
    Input status;  

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