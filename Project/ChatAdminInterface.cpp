#include "ChatAdminInterface.h"

chat::Results ChatAdminInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    /**
     * код приложения администраторского интерфейса
    */
    return chat::empty;
}