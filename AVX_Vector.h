#pragma once
#include <immintrin.h>

#include <initializer_list>
#include <iostream>
#include <memory>
#include <ranges>

template<typename T>
class AVX_Vector
{
private:
	size_t size;


public:
	alignas(32) T* data;  // выравнивание данных по 32 байта

	AVX_Vector() : size(4), data(nullptr) {}
	AVX_Vector(const std::initializer_list<T>& initList) : size(initList.size())
	{
		data = new T[size];

		for (auto [i, value] : initList | std::views::enumerate)
		{
			data[i] = value;
		}
	}

	AVX_Vector(size_t data_size) : size(data_size)
	{
		data = new T[size];
	}
	AVX_Vector(const AVX_Vector& other) : size(other.size)
	{
		data = nullptr;

		if (other.size > 0)
		{
			data = new T[other.size];
			std::memcpy(data, other.data, other.size * sizeof(T));
		}
	}



	T& operator[](size_t idx)
	{
		if (idx >= size)
		{
			throw std::out_of_range("Индекс выходит за гранцы массива!");
		}
		return data[idx];
	}
	const T& operator[] (size_t idx) const
	{
		if (idx >= size)
		{
			throw std::out_of_range("Индекс выходит за гранцы массива!");
		}
		return data[idx];
	}



	const AVX_Vector& operator= (const AVX_Vector& rhs)
	{

		if (this == &rhs)
			return *this;

		delete[] data;

		size = rhs.size;
		data = new T[size];
		for (size_t i = 0; i < size; ++i)
		{
			data[i] = rhs.data[i];
		}

		return *this;
	}

	const AVX_Vector& operator=(AVX_Vector&& rhs) noexcept
	{
		if (this == &rhs)
			return *this;

		delete[] data;

		data = rhs.data;
		size = rhs.size;

		rhs.data = nullptr;
		rhs.size = 0;

		return *this;
	}

	size_t getSize() const
	{
		return size;
	}

	~AVX_Vector()
	{
		delete[] data;
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const AVX_Vector<T>& arr)
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
AVX_Vector<T> operator+(const AVX_Vector<T>& v_1, const AVX_Vector<T>& v_2)
{
	if (v_1.getSize() != v_2.getSize()) {
		throw std::invalid_argument("Размеры векторов не совпадают!");
	}

	AVX_Vector<T> sum(v_1.getSize());
	size_t size = v_1.getSize();
	size_t i = 0;

	// поскольку у меня шаблонный класс, а у AVX нет шаблонов,
	// надо проверить во время компиляции какой тип данных в векторе
	// с помощью std::is_same_v, который сравнивает тип T и float. 
	// Если тип Т является флоат, то тогда будет выполнено это условие ==true

	if constexpr (std::is_same_v<T, float>)
	{
		// 256/32 = 8 бит 
		for (; i + 8 <= size; i += 8)
		{
			__m256 a = _mm256_loadu_ps(&v_1[i]);
			__m256 b = _mm256_loadu_ps(&v_2[i]);
			__m256 c = _mm256_add_ps(a, b);
			_mm256_storeu_ps(&sum[i], c);
		}
	}
	else if constexpr (std::is_same_v<T, double>)
	{
		for (; i + 4 <= size; i += 4)
		{
			__m256d a = _mm256_loadu_pd(&v_1[i]);
			__m256d b = _mm256_loadu_pd(&v_2[i]);
			__m256d c = _mm256_add_pd(a, b);
			_mm256_storeu_pd(&sum[i], c);
		}
	}
	else if constexpr (std::is_same_v<T, int32_t>)
	{
		for (; i + 8 <= size; i += 8)
		{
			// reinterpret_cast нужен, потому что & _v1[i] — это int*, а AVX2 функции хотят __m256i*

			__m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&v_1[i]));
			__m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&v_2[i]));
			__m256i c = _mm256_add_epi32(a, b);
			// epi32 — это инструкция из AVX2 (AVX обычный не поддерживает целочисленную арифметику)
			_mm256_storeu_si256(reinterpret_cast<__m256i*>(&sum[i]), c);
		}
	}
	else // любой другой тип — обычный способ
	{
		for (; i < size; ++i)
			sum[i] = v_1[i] + v_2[i];
	}

	// обработка "хвоста" — элементы, которые не поместились в AVX-регистры
	for (; i < size; ++i)
	{
		sum[i] = v_1[i] + v_2[i];
	}

	return sum;
}


template <typename T>
AVX_Vector<T> operator-(const AVX_Vector<T>& v_1, const AVX_Vector<T>& v_2)
{
	if (v_1.getSize() != v_2.getSize()) {
		throw std::invalid_argument("Размеры векторов не совпадают!");
	}

	AVX_Vector<T> minus(v_1.getSize());
	size_t size = v_1.getSize();
	size_t i = 0;

	if constexpr (std::is_same_v<T, float>)
	{
		for (; i + 8 <= size; i += 8)
		{
			__m256 a = _mm256_loadu_ps(&v_1[i]);
			__m256 b = _mm256_loadu_ps(&v_2[i]);
			__m256 c = _mm256_sub_ps(a, b);
			_mm256_storeu_ps(&minus[i], c);
		}
	}
	else if constexpr (std::is_same_v<T, double>)
	{
		for (; i + 4 <= size; i += 4)
		{
			__m256d a = _mm256_loadu_pd(&v_1[i]);
			__m256d b = _mm256_loadu_pd(&v_2[i]);
			__m256d c = _mm256_sub_pd(a, b);
			_mm256_storeu_pd(&minus[i], c);
		}
	}
	else if constexpr (std::is_same_v<T, int32_t>)
	{
		for (; i + 8 <= size; i += 8)
		{
			__m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&v_1[i]));
			__m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&v_2[i]));
			__m256i c = _mm256_sub_epi32(a, b);
			_mm256_storeu_si256(reinterpret_cast<__m256i*>(&minus[i]), c);
		}
	}
	else
	{
		for (; i < size; ++i)
			minus[i] = v_1[i] - v_2[i];
	}

	// Обработка хвоста
	for (; i < size; ++i)
	{
		minus[i] = v_1[i] - v_2[i];
	}

	return minus;
}



template <typename T>
T operator*(const AVX_Vector<T>& v_1, const AVX_Vector<T>& v_2)
{
	if (v_1.getSize() != v_2.getSize()) {
		throw std::invalid_argument("Размеры векторов не совпадают!");
	}

	T mult = 0;
	size_t size = v_1.getSize();
	size_t i = 0;

	if constexpr (std::is_same_v<T, float>)
	{
		for (; i + 8 <= size; i += 8)
		{
			__m256 a = _mm256_loadu_ps(&v_1[i]);
			__m256 b = _mm256_loadu_ps(&v_2[i]);
			__m256 c = _mm256_mul_ps(a, b);
			mult += _mm256_reduce_add_ps(c);  // В этом месте суммирование всех элементов вектора c
		}
	}
	else if constexpr (std::is_same_v<T, double>)
	{
		for (; i + 4 <= size; i += 4)
		{
			__m256d a = _mm256_loadu_pd(&v_1[i]);
			__m256d b = _mm256_loadu_pd(&v_2[i]);
			__m256d c = _mm256_mul_pd(a, b);
			mult += _mm256_reduce_add_pd(c);
		}
	}
	else if constexpr (std::is_same_v<T, int32_t>)
	{
		for (; i + 8 <= size; i += 8)
		{
			__m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&v_1[i]));
			__m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&v_2[i]));
			__m256i c = _mm256_mullo_epi32(a, b);  // c = [a1 * b1, a2 * b2, a3 * b3, a4 * b4, a5 * b5, a6 * b6, a7 * b7, a8 * b8]

			// этот дебил не умеет по-человечески одной командой складывать все целочисленные элементы вектора
			// // складываем попарно соседние элементы внутри регистра, т.к. 
			// надо (а1*б1, а2*б2) сложить, чтоб получить не число, а вектор 
			__m256i hadd1 = _mm256_hadd_epi32(c, c);  // в векторе с делаем 8/2 = 4 пары и их складываем и записываем в первые 4 индекса (первая стадия)
			//c = [(a1 * b1 + a2 * b2), (a3 * b3 + a4 * b4), (a5 * b5 + a6 * b6), (a7 * b7 + a8 * b8), 0, 0, 0, 0]
			//hadd1 = [(a1 * b1 + a2 * b2), (a3 * b3 + a4 * b4), (a5 * b5 + a6 * b6), (a7 * b7 + a8 * b8)]

			__m256i hadd2 = _mm256_hadd_epi32(hadd1, hadd1);  // снова делим на 4/2 = 2 пары и записываем в первые 2 индекса
			//hadd1 = [((a1 * b1 + a2 * b2) + (a3 * b3 + a4 * b4)), ((a5 * b5 + a6 * b6) + (a7 * b7 + a8 * b8)), 0, 0, 0, 0, 0, 0]
			//hadd2 = [((a1 * b1 + a2 * b2) + (a3 * b3 + a4 * b4)), ((a5 * b5 + a6 * b6) + (a7 * b7 + a8 * b8))]

			mult += _mm256_extract_epi32(hadd2, 0) + _mm256_extract_epi32(hadd2, 1);
		}
	}
	else
	{
		for (; i < size; ++i)
		{
			mult += v_1[i] * v_2[i];
		}
	}

	// обработка хвоста
	for (; i < size; ++i)
	{
		mult += v_1[i] * v_2[i];
	}

	return mult;  
}
