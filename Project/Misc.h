#pragma once
#include <string>

class Misc
{
public:
    /// @brief Проверяет строку на соответствие шаблону: A-Z a-z А-Я а-я 0-9
    /// @param str
    /// @return bool
    static bool CheckString(std::string str);
};