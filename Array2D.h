#pragma once
#include <initializer_list>
#include <vector>
#include <iostream>
#include <stdexcept>


template<typename T>
class Array2D
{
private:
    std::vector<std::vector<T>> data;

public:
    Array2D() = default;

    //Это конструктор, чтоб можно было красиво инициализировать массив { {...},{...} }
    Array2D(const std::initializer_list<std::initializer_list<T>>& list)
    {
        data.reserve(list.size());

        for (const auto& row : list)
        {
            // передаю диапазон строк в листе, потому что  data.emplace_back ждёт аргументы типа вектора, а не листа, поэтому не push_back
            //emplace_back конструирует объект на месте (внутри вектора), используя переданные аргументы как параметры конструктора для создания вектора
            data.emplace_back(row.begin(), row.end());
        }
    }

    //Это конструктор для создания массива по его размерам.
    //используется member initializer list
    //std::vector<T>(cols) создаёт один вектор - строку из cols элементов(значения по умолчанию для T).
    //data(rows, ...) создаёт вектор из rows копий этого вектора - строки.
    Array2D(size_t rows, size_t cols) : data(rows, std::vector<T>(cols)) {}

    size_t GetCols() const
    {
        const size_t cols = data[0].size();

        for (const auto& row : data)
        {
            if (row.size() != cols)
            {
                throw std::invalid_argument("Нужно, чтобы все строки были одного размера!");
            }
        }

        return cols;
    }
    size_t GetRows() const
    {
        return data.size();
    }


    // доступ по индексу
    // авто, потому что сначала он вернет вектор, а потом в векторе тип Т, когда мы обратимся arr [][]
    auto& operator[](size_t idx)
    {
        return data[idx];
    }
    const auto& operator[] (size_t idx) const
    {
        return data[idx];
    }

    // data — private, но friend разрешает доступ
    friend std::ostream& operator<< (std::ostream& out, const Array2D<T>& arr)
    {
        out << "{";
        for (size_t r = 0; r < arr.GetRows(); ++r)
        {
            out << "{";
            for (size_t c = 0; c < arr.GetCols(); ++c)
            {
                out << arr.data[r][c];
                if (c < arr.GetCols() - 1)
                {
                    out << ", ";
                }
            }
            out << "}";
        }
        out << "}";
        return out;
    }


    ~Array2D() {}
};

template <typename T>
Array2D<T> operator+(const Array2D<T>& a_1, const Array2D<T>& a_2)
{
    if (a_1.GetRows() != a_2.GetRows() || a_1.GetCols() != a_2.GetCols())
    {
        throw std::invalid_argument("Размеры матриц не совпадают!");
    }

    Array2D<T> sum(a_1.GetRows(), a_1.GetCols());

    for (size_t r = 0; r < a_1.GetRows(); ++r)
    {
        for (size_t c = 0; c < a_1.GetCols(); ++c)
        {
            sum[r][c] = a_1[r][c] + a_2[r][c];
        }
    }

    return sum;
}

template <typename T>
Array2D<T> operator- (const Array2D<T>& a_1, const Array2D<T>& a_2)
{
    if (a_1.GetRows() != a_2.GetRows() || a_1.GetCols() != a_2.GetCols())
    {
        throw std::invalid_argument("Размеры матриц не совпадают!");
    }

    Array2D<T> sum(a_1.GetRows(), a_1.GetCols());

    for (size_t r = 0; r < a_1.GetRows(); ++r)
    {
        for (size_t c = 0; c < a_1.GetCols(); ++c)
        {
            sum[r][c] = a_1[r][c] - a_2[r][c];
        }
    }

    return sum;
}

template <typename T>
Array2D<T> operator*(const Array2D<T>& a_1, const Array2D<T>& a_2)
{
    if (a_1.GetRows() != a_2.GetRows() || a_1.GetCols() != a_2.GetCols())
    {
        throw std::invalid_argument("Размеры матриц не совпадают!");
    }

    Array2D<T> sum(a_1.GetRows(), a_1.GetCols());

    for (size_t r = 0; r < a_1.GetRows(); ++r)
    {
        for (size_t c = 0; c < a_1.GetCols(); ++c)
        {
            sum[r][c] = a_1[r][c] * a_2[r][c];
        }
    }

    return sum;
}

template <typename T>
Array2D<T> operator/(const Array2D<T>& a_1, const Array2D<T>& a_2)
{
    if (a_1.GetRows() != a_2.GetRows() || a_1.GetCols() != a_2.GetCols())
    {
        throw std::invalid_argument("Размеры матриц не совпадают!");
    }

    Array2D<T> sum(a_1.GetRows(), a_1.GetCols());

    for (size_t r = 0; r < a_1.GetRows(); ++r)
    {
        for (size_t c = 0; c < a_1.GetCols(); ++c)
        {
            if (a_2[r][c] == 0)
            {
                throw std::runtime_error("Нельзя делить на ноль!");
            }
            sum[r][c] = a_1[r][c] / a_2[r][c];
        }
    }

    return sum;
}

