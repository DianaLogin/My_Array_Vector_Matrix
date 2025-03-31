#pragma once
#include <initializer_list>
#include <vector>
#include <iostream>
#include <stdexcept>

template<typename T>
class Array
{
private:
    std::vector<T> data;

public:
    Array() = default;
    Array(const std::initializer_list<T>& list) : data(list) { data.reserve(list.size()); }
    Array(size_t data_size) : data(data_size) {}

    // доступ по индексу
    T& operator[](size_t idx)
    {
        if (idx >= data.size())
        {
            throw std::out_of_range("»ндекс выходит за гранцы массива!");
        }
        return data[idx];
    }
    const T& operator[] (size_t idx) const
    {
        if (idx >= data.size())
        {
            throw std::out_of_range("»ндекс выходит за гранцы массива!");
        }
        return data[idx];
    }

    template<typename T>
    const Array<T>& operator= (Array<T>&& rhs) noexcept
    {
        this->data = rhs.data;
        return *this;
    }


    size_t getSize() const
    {
        return data.size();
    }


    ~Array() {}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& arr)
{
    out << "{";
    for (size_t i = 0; i < arr.getSize(); ++i)
    {
        out << arr[i];
        if (i < arr.getSize() - 1)
        {
            out << ",";
        }
    }
    out << "}";
    return out;
}


template <typename T>
Array<T> operator+ (const Array<T>& a_1, const Array<T>& a_2)
{
    if (a_1.getSize() != a_2.getSize())
    {
        throw std::invalid_argument("–азмеры складываемых массивов не совпадают!");
    }

    Array<T> sum(a_1.getSize());

    for (size_t i = 0; i < a_1.getSize(); ++i)
    {
        sum[i] = a_1[i] + a_2[i];
    }

    return sum;
}

template <typename T>
Array<T> operator- (const Array<T>& a_1, const Array<T>& a_2)
{
    if (a_1.getSize() != a_2.getSize())
    {
        throw std::invalid_argument("–азмеры вычитаемых массивов не совпадают!");
    }

    Array<T> sum(a_1.getSize());

    for (size_t i = 0; i < a_1.getSize(); ++i)
    {
        sum[i] = a_1[i] - a_2[i];
    }

    return sum;
}

template <typename T>
Array<T> operator* (const Array<T>& a_1, const Array<T>& a_2)
{
    if (a_1.getSize() != a_2.getSize())
    {
        throw std::invalid_argument("–азмеры вычитаемых массивов не совпадают!");
    }

    Array<T> sum(a_1.getSize());

    for (size_t i = 0; i < a_1.getSize(); ++i)
    {
        sum[i] = a_1[i] * a_2[i];
    }

    return sum;
}

template <typename T>
Array<T> operator/ (const Array<T>& a_1, const Array<T>& a_2)
{
    if (a_1.getSize() != a_2.getSize())
    {
        throw std::invalid_argument("–азмеры вычитаемых массивов не совпадают!");
    }

    Array<T> sum(a_1.getSize());

    for (size_t i = 0; i < a_1.getSize(); ++i)
    {
        if (a_2[i] == 0)
        {
            throw std::runtime_error("ƒеление на ноль запрещено!");
        }
        sum[i] = a_1[i] / a_2[i];
    }

    return sum;
}