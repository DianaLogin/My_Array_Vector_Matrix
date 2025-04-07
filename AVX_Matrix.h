#pragma once
#include <immintrin.h>

#include <array>
#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <string>
#include <cassert>
#include "AVX_Vector.h"

template <typename T, size_t Rows, size_t Cols>
class AVX_Matrix
{
private:
    std::array<T, Rows* Cols> data;

    constexpr size_t index(size_t row, size_t col) const
    {
        return row * Cols + col;
    }

public:
    AVX_Matrix() = default;

    AVX_Matrix(const std::initializer_list<std::initializer_list<T>>& list)
    {
        size_t i = 0;
        for (const auto& row : list)
        {
            for (const auto& val : row)
            {
                data[i++] = val;
            }
        }
    }

    T& operator()(size_t row, size_t col)
    {
        return data[index(row, col)];
    }
    const T& operator()(size_t row, size_t col) const
    {
        return data[index(row, col)];
    }



    AVX_Matrix<T, Rows, Cols> operator+(const AVX_Matrix<T, Rows, Cols>& other) const
    {
        AVX_Matrix<T, Rows, Cols> summa;

        size_t j = 0;

        for (size_t i = 0; i < Rows; ++i)
        {
            j = 0;

            if constexpr (std::is_same_v<T, float>)
            {
                for (; j + 8 <= Cols; j += 8)
                {
                    __m256 mat1 = _mm256_loadu_ps(&(*this)(i, j));
                    __m256 mat2 = _mm256_loadu_ps(&other(i, j));
                    __m256 sum = _mm256_add_ps(mat1, mat2);
                    _mm256_storeu_ps(&summa(i, j), sum);
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                for (; j + 4 <= Cols; j += 4)
                {
                    __m256d mat1 = _mm256_loadu_pd(&(*this)(i, j));
                    __m256d mat2 = _mm256_loadu_pd(&other(i, j));
                    __m256d sum = _mm256_add_pd(mat1, mat2);
                    _mm256_storeu_pd(&summa(i, j), sum);
                }
            }
            else if constexpr (std::is_same_v<T, int32_t>)
            {
                for (; j + 8 <= Cols; j += 8)
                {
                    __m256i mat1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&(*this)(i, j)));
                    __m256i mat2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&other(i, j)));
                    __m256i sum = _mm256_add_epi32(mat1, mat2);
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&summa(i, j)), sum);
                }
            }

            for (; j < Cols; ++j)
            {
                summa(i, j) = (*this)(i, j) + other(i, j);
            }
        }

        return summa;
    }



    AVX_Matrix<T, Rows, Cols> operator-(const AVX_Matrix<T, Rows, Cols>& other) const
    {
        AVX_Matrix<T, Rows, Cols> result;

        size_t j = 0;

        for (size_t i = 0; i < Rows; ++i)
        {
            j = 0;

            if constexpr (std::is_same_v<T, float>)
            {
                for (; j + 8 <= Cols; j += 8)
                {
                    __m256 a = _mm256_loadu_ps(&(*this)(i, j));
                    __m256 b = _mm256_loadu_ps(&other(i, j));
                    __m256 sub = _mm256_sub_ps(a, b);
                    _mm256_storeu_ps(&result(i, j), sub);
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                for (; j + 4 <= Cols; j += 4)
                {
                    __m256d a = _mm256_loadu_pd(&(*this)(i, j));
                    __m256d b = _mm256_loadu_pd(&other(i, j));
                    __m256d sub = _mm256_sub_pd(a, b);
                    _mm256_storeu_pd(&result(i, j), sub);
                }
            }
            else if constexpr (std::is_same_v<T, int32_t>)
            {
                for (; j + 8 <= Cols; j += 8)
                {
                    __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&(*this)(i, j)));
                    __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&other(i, j)));
                    __m256i sub = _mm256_sub_epi32(a, b);
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&result(i, j)), sub);
                }
            }

            for (; j < Cols; ++j)
            {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }

        return result;
    }



    AVX_Matrix<T, Rows, Cols> operator*(T scalar) const
    {
        AVX_Matrix<T, Rows, Cols> result;

        size_t j = 0;

        for (size_t i = 0; i < Rows; ++i)
        {
            j = 0;

            if constexpr (std::is_same_v<T, float>)
            {
                __m256 scalar_vec = _mm256_set1_ps(scalar);
                for (; j + 8 <= Cols; j += 8)
                {
                    __m256 vec = _mm256_loadu_ps(&(*this)(i, j));
                    __m256 prod = _mm256_mul_ps(vec, scalar_vec);
                    _mm256_storeu_ps(&result(i, j), prod);
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                __m256d scalar_vec = _mm256_set1_pd(scalar);
                for (; j + 4 <= Cols; j += 4)
                {
                    __m256d vec = _mm256_loadu_pd(&(*this)(i, j));
                    __m256d prod = _mm256_mul_pd(vec, scalar_vec);
                    _mm256_storeu_pd(&result(i, j), prod);
                }
            }
            else if constexpr (std::is_same_v<T, int32_t>)
            {
                __m256i scalar_vec = _mm256_set1_epi32(scalar);
                for (; j + 8 <= Cols; j += 8)
                {
                    __m256i vec = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&(*this)(i, j)));
                    __m256i prod = _mm256_mullo_epi32(vec, scalar_vec);
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(&result(i, j)), prod);
                }
            }

            for (; j < Cols; ++j)
            {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }

        return result;
    }


    friend std::ostream& operator<<(std::ostream& out, const AVX_Matrix<T, Rows, Cols>& m)
    {
        out << "{\n";
        for (size_t i = 0; i < Rows; ++i)
        {
            out << "  {";
            for (size_t j = 0; j < Cols; ++j)
            {
                out << m(i, j);
                if (j < Cols - 1)
                {
                    out << ", ";
                }
            }
            out << "}";
            if (i < Rows - 1)
            {
                out << ",";
            }
            out << "\n";
        }
        out << "}";
        return out;
    }
};

//C(i,j) = ∑ m_1(i,k) * m_2(k,j)
template<typename T, size_t Rows_1, size_t Cols_1, size_t Rows_2, size_t Cols_2>
AVX_Matrix<T, Rows_1, Cols_2> operator*(const AVX_Matrix<T, Rows_1, Cols_1>& m_1, const AVX_Matrix<T, Rows_2, Cols_2>& m_2)
{
    assert(Cols_1 == Rows_2);

    AVX_Matrix<T, Rows_1, Cols_2> result;

    for (size_t i = 0; i < Rows_1; ++i)
    {
        for (size_t j = 0; j < Cols_2; ++j)
        {
            T sum = 0;
            size_t k = 0;

            if constexpr (std::is_same_v<T, float>)
            {
                __m256 mult = _mm256_setzero_ps();
                for (; k + 8 <= Cols_1; k += 8)
                {
                    __m256 row_1 = _mm256_loadu_ps(&m_1(i, k));
                    __m256 col_2 = _mm256_loadu_ps(&m_2(k, j));
                    mult = _mm256_add_ps(mult, _mm256_mul_ps(row_1, col_2));
                }
                alignas(32) float alignv[8]; // вырвнивание
                _mm256_store_ps(alignv, mult); // копирует элементы авх-вектора mult в обычный массив alignv
                for (int t = 0; t < 8; ++t)
                {
                    sum += alignv[t]; // все элементы по одному как обычно в обычном массиве складываем
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                __m256d mult = _mm256_setzero_pd();
                for (; k + 4 <= Cols_1; k += 4)
                {
                    __m256d a = _mm256_loadu_pd(&m_1(i, k));
                    __m256d b = _mm256_loadu_pd(&m_2(k, j));
                    mult = _mm256_add_pd(mult, _mm256_mul_pd(a, b));
                }
                alignas(32) double align[4];
                _mm256_store_pd(align, mult);
                for (int t = 0; t < 4; ++t)
                {
                    sum += align[t];
                }
            }
            else if constexpr (std::is_same_v<T, int32_t>)
            {
                __m256i mult = _mm256_setzero_si256();
                for (; k + 8 <= Cols_1; k += 8)
                {
                    __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&m_1(i, k)));
                    __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&m_2(k, j)));
                    __m256i c = _mm256_mullo_epi32(a, b);
                    mult = _mm256_add_epi32(mult, c);
                }
                alignas(32) int align[8];
                _mm256_store_si256(reinterpret_cast<__m256i*>(align), mult);
                for (int t = 0; t < 8; ++t)
                {
                    sum += align[t];
                }
            }
            else // для остальных типов
            {
                for (; k < Cols_1; ++k)
                {
                    sum += m_1(i, k) * m_2(k, j);
                }
                result(i, j) = sum;
                continue;
            }

            // остатки
            for (; k < Cols_1; ++k)
            {
                sum += m_1(i, k) * m_2(k, j);
            }

            result(i, j) = sum;
        }
    }

    return result;
}

template<typename T, size_t Rows, size_t Cols>
AVX_Vector<T> operator*(const AVX_Matrix<T, Rows, Cols>& matrix, const AVX_Vector<T>& vector)
{
    assert(vector.getSize() == Cols);

    AVX_Vector<T> multiply(Rows);

    for (size_t i = 0; i < Rows; ++i)
    {
        T sum = 0;
        size_t j = 0;

        if constexpr (std::is_same_v<T, float>)
        {
            __m256 vec_sum = _mm256_setzero_ps();
            for (; j + 8 <= Cols; j += 8)
            {
                __m256 mat_row = _mm256_loadu_ps(&matrix(i, j));
                __m256 vec_col = _mm256_loadu_ps(&vector[j]);
                vec_sum = _mm256_add_ps(vec_sum, _mm256_mul_ps(mat_row, vec_col));
            }
            alignas(32) float align[8];
            _mm256_store_ps(align, vec_sum);
            for (int t = 0; t < 8; ++t)
            {
                sum += align[t];
            }
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            __m256d vec_sum = _mm256_setzero_pd();
            for (; j + 4 <= Cols; j += 4)
            {
                __m256d mat_row = _mm256_loadu_pd(&matrix(i, j));
                __m256d vec_col = _mm256_loadu_pd(&vector[j]);
                vec_sum = _mm256_add_pd(vec_sum, _mm256_mul_pd(mat_row, vec_col));
            }
            alignas(32) double align[4];
            _mm256_store_pd(align, vec_sum);
            for (int t = 0; t < 4; ++t)
            {
                sum += align[t];
            }
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            __m256i vec_sum = _mm256_setzero_si256();
            for (; j + 8 <= Cols; j += 8)
            {
                __m256i mat_row = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&matrix(i, j)));
                __m256i vec_col = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&vector[j]));
                __m256i mult = _mm256_mullo_epi32(mat_row, vec_col);
                vec_sum = _mm256_add_epi32(vec_sum, mult);
            }
            alignas(32) int align[8];
            _mm256_store_si256(reinterpret_cast<__m256i*>(align), vec_sum);
            for (int t = 0; t < 8; ++t)
            {
                sum += align[t];
            }
        }

        // oстатки
        for (; j < Cols; ++j)
        {
            sum += matrix(i, j) * vector[j];
        }

        multiply[i] = sum;
    }

    return multiply;
}


template<typename T, size_t Rows, size_t Cols>
AVX_Vector<T> operator*(const AVX_Vector<T>& vector, const AVX_Matrix<T, Rows, Cols>& matrix)
{
    assert(vector.getSize() == Rows);

    AVX_Vector<T> multiply(Cols);

    for (size_t j = 0; j < Cols; ++j)
    {
        T sum = 0;
        size_t i = 0;

        if constexpr (std::is_same_v<T, float>)
        {
            __m256 vec_sum = _mm256_setzero_ps();
            for (; i + 8 <= Rows; i += 8)
            {
                __m256 vec_vals = _mm256_loadu_ps(&vector[i]);
                __m256 mat_vals = _mm256_loadu_ps(&matrix(i, j));
                vec_sum = _mm256_add_ps(vec_sum, _mm256_mul_ps(vec_vals, mat_vals));
            }
            alignas(32) float align[8];
            _mm256_store_ps(align, vec_sum);
            for (int t = 0; t < 8; ++t)
            {
                sum += align[t];
            }
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            __m256d vec_sum = _mm256_setzero_pd();
            for (; i + 4 <= Rows; i += 4)
            {
                __m256d vec_vals = _mm256_loadu_pd(&vector[i]);
                __m256d mat_vals = _mm256_loadu_pd(&matrix(i, j));
                vec_sum = _mm256_add_pd(vec_sum, _mm256_mul_pd(vec_vals, mat_vals));
            }
            alignas(32) double align[4];
            _mm256_store_pd(align, vec_sum);
            for (int t = 0; t < 4; ++t)
            {
                sum += align[t];
            }
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            __m256i vec_sum = _mm256_setzero_si256();
            for (; i + 8 <= Rows; i += 8)
            {
                __m256i vec_vals = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&vector[i]));
                __m256i mat_vals = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&matrix(i, j)));
                __m256i prod = _mm256_mullo_epi32(vec_vals, mat_vals);
                vec_sum = _mm256_add_epi32(vec_sum, prod);
            }
            alignas(32) int align[8];
            _mm256_store_si256(reinterpret_cast<__m256i*>(align), vec_sum);
            for (int t = 0; t < 8; ++t)
            {
                sum += align[t];
            }
        }

        // Остатки
        for (; i < Rows; ++i)
        {
            sum += vector[i] * matrix(i, j);
        }

        multiply[j] = sum;
    }

    return multiply;
}





