#pragma once
#include <iostream>
#include <string>

/// @brief Реализует ввод данных пользователем
/// @return T - Введенные данные
template <typename T>
T getInput(std::string message);

template <typename T>
inline T getInput(std::string message)
{
    T out;
    std::cout << message;
    std::cin >> out;
    std::cin.clear();
    while (std::cin.get() != '\n') {}
    return out;
}