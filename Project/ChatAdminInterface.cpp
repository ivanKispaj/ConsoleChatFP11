#include "ChatAdminInterface.h"

ChatAdminInterface::ChatAdminInterface(DB *_db) : IChatInterface(_db) {}

Results ChatAdminInterface::run()
{
    return Results();
}