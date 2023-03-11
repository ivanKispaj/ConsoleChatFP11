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

    // append an element to end of the array
    void append(const T &value)
    {
        _size++;
        std::unique_ptr<T[]> newArray = std::make_unique<T[]>(_size);
        if (_size <= 1)
        {
            newArray[0] = value;
        }
        else
        {
            for (int i = 0; i < _size - 1; i++)
            {
                newArray[i] = _array[i];
            }
            newArray[_size - 1] = value;
        }

        _array = std::move(newArray);
    }

    /*
overloaded index taking operator.
Returns a pointer to the array value by index,
nullptr if array is empry or index out if range..
*/
    T &operator[](const int index)
    {

        if (index >= 0 && index < _size)
        {
            return _array[index];
        }
        if (isEmpty())
        {
            throw;
        }
        throw std::overflow_error("Index out of range!!!");
    }

    T &operator[](const int index) const
    {

        if (index >= 0 && index < _size)
        {
            return _array[index];
        }
        if (isEmpty())
        {
            throw;
        }
        throw std::overflow_error("Index out of range!!!");
    }
};
