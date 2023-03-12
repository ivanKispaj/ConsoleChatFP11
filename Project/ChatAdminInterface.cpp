#include "ChatAdminInterface.h"

ChatAdminInterface::ChatAdminInterface(DB *_db) : IChatInterface(_db) {}

int ChatAdminInterface::run()
{
    return 0;
}