#include "ChatAdminInterface.h"

ChatAdminInterface::ChatAdminInterface(DB *_db) : IChatInterface(_db) {}

Input ChatAdminInterface::run()
{
    return Input();
}