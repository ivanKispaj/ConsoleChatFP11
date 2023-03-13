#pragma once
#include "DB.h"
#include <iostream>

class IChatInterface
{
protected:
    DB *db;
    User user;

public:
    IChatInterface() = default;
    IChatInterface(DB *_db)
    {
        db = _db;
    }
    virtual ~IChatInterface() = default;
    virtual int run() = 0;
    int login();
};