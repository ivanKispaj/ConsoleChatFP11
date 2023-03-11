//
// DBCore.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once
#include <iostream>

template <typename T>
class DBCore
{
    int _size{0}; // size of array
    std::unique_ptr<T[]> _array = nullptr;

public:
    // Init default
    DBCore<T>() : _array(nullptr), _size(0){};

    // deinit default
    ~DBCore() = default;

    //  Copy construction
    DBCore<T>(const DBCore &array)
    {
        _size = array._size;
        _array = std::make_unique<T[]>(array._size);
        for (int i = 0; i < _size; i++)
        {
            _array[i] = array._array[i];
        }
    }

    // return size of array
    int count() const
    {
        return _size;
    }

    // return true if array is empty
    bool isEmpty() const
    {
        return _size == 0;
    }
};
