#include <iostream> 
#include "Array.h"
#include "Vector.h"
#include "Matrix.h"
#include "Array2D.h"
#include <locale>

int main()
{
    setlocale(LC_ALL, "Russian");
    try
    {

        Vector<int> vec_1 = { 1, 3, 5, 7, 78 };
        Vector<int> vec_2 = { 1, 3, 8, 7, 7 };
        std::cout << "сложение Vector:  " << vec_1 << " + " << vec_2 << " = " << vec_1 + vec_2 << std::endl;
        std::cout << "вычитание Vector:  " << vec_1 << " - " << vec_2 << " = " << vec_1 - vec_2 << std::endl;
        std::cout << "умножение Vector:  " << vec_1 << " * " << vec_2 << " = " << vec_1 * vec_2 << "\n" << std::endl;


        Matrix<int, 2, 3> mat = {
                                {1, 2, 3},
                                {4, 5, 6}
                                };

        // Вектор-столбец
        Vector<int> vec_col = { 1, 2, 3 };

        // Вектор-строка 
        Vector<int> vec_row = { 1, 2 };

        // Умножение матрицы на вектор-столбец
        auto res1 = mat * vec_col; // Будет вектор из 2 элементов
        std::cout << "Matrix * Vector: " << res1 << std::endl;

        // Умножение вектора-строки на матрицу
        auto res2 = vec_row * mat; // Будет вектор из 3 элементов
        std::cout << "Vector * Matrix: " << res2 << std::endl;

        Array<int> arr_1 = { 1, 3, 5, 7, 78 };
        Array<int> arr_2 = { 1, 3, 8, 7, 7 };
        std::cout << arr_1[3] << "\n";
        arr_1 = arr_2;
        std::cout << "сложение Array:  " << arr_1 << " + " << arr_2 << " = " << arr_1 + arr_2 << std::endl;
        std::cout << "вычитание Array:  " << arr_1 << " - " << arr_2 << " = " << arr_1 - arr_2 << std::endl;
        std::cout << "умножение Array:  " << arr_1 << " * " << arr_2 << " = " << arr_1 * arr_2 << std::endl;
        std::cout << "деление Array : " << arr_1 << " / " << arr_2 << " = " << arr_1 / arr_2 << "\n" << std::endl;

        Array2D<int> arr2d_1 = { { 1, 3 }, { 1, 3} };
        Array2D<int> arr2d_2 = { { 1, 0}, { 1, 3} };
        std::cout << "сложение Array2D" << arr2d_1 << " + " << arr2d_2 << " = " << (arr2d_1 + arr2d_2) << std::endl;
        std::cout << "вычитание Array2D" << arr2d_1 << " - " << arr2d_2 << " = " << (arr2d_1 - arr2d_2) << std::endl;
        std::cout << "умножение Array2D" << arr2d_1 << " * " << arr2d_2 << " = " << (arr2d_1 * arr2d_2) << std::endl;
        std::cout << "деление Array2D" << arr2d_1 << " / " << arr2d_2 << " = " << (arr2d_1 / arr2d_2) <<"\n" << std::endl;


        

    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}