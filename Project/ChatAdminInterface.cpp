#include "ChatAdminInterface.h"

Results ChatAdminInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    login();
    return empty;
}