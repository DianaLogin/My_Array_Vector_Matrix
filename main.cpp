#include <iostream> 
#include "Array.h"
#include "AVX_Vector.h"
#include "Vector.h"
#include "Matrix.h"
#include "AVX_Matrix.h"
#include "Array2D.h"
#include <locale>
#include <chrono>

int main()
{
    setlocale(LC_ALL, "Russian");
    try
    {

    


        //Matrix<int, 2, 3> mat = {
        //                        {1, 2, 3},
        //                        {4, 5, 6}
        //                        };

        //// Вектор-столбец
        //Vector<int> vec_col = { 1, 2, 3 };

        //// Вектор-строка 
        //Vector<int> vec_row = { 1, 2 };

        //// Умножение матрицы на вектор-столбец
        //auto res1 = mat * vec_col; // Будет вектор из 2 элементов
        //std::cout << "Matrix * Vector: " << res1 << std::endl;

        //// Умножение вектора-строки на матрицу
        //auto res2 = vec_row * mat; // Будет вектор из 3 элементов
        //std::cout << "Vector * Matrix: " << res2 << std::endl;

        //Array<int> arr_1 = { 1, 3, 5, 7, 78 };
        //Array<int> arr_2 = { 1, 3, 8, 7, 7 };
        //std::cout << arr_1[3] << "\n";
        //arr_1 = arr_2;
        //std::cout << "сложение Array:  " << arr_1 << " + " << arr_2 << " = " << arr_1 + arr_2 << std::endl;
        //std::cout << "вычитание Array:  " << arr_1 << " - " << arr_2 << " = " << arr_1 - arr_2 << std::endl;
        //std::cout << "умножение Array:  " << arr_1 << " * " << arr_2 << " = " << arr_1 * arr_2 << std::endl;
        //std::cout << "деление Array : " << arr_1 << " / " << arr_2 << " = " << arr_1 / arr_2 << "\n" << std::endl;

        //Array2D<int> arr2d_1 = { { 1, 3 }, { 1, 3} };
        //Array2D<int> arr2d_2 = { { 1, 0}, { 1, 3} };
        //std::cout << "сложение Array2D" << arr2d_1 << " + " << arr2d_2 << " = " << (arr2d_1 + arr2d_2) << std::endl;
        //std::cout << "вычитание Array2D" << arr2d_1 << " - " << arr2d_2 << " = " << (arr2d_1 - arr2d_2) << std::endl;
        //std::cout << "умножение Array2D" << arr2d_1 << " * " << arr2d_2 << " = " << (arr2d_1 * arr2d_2) << std::endl;
        //std::cout << "деление Array2D" << arr2d_1 << " / " << arr2d_2 << " = " << (arr2d_1 / arr2d_2) <<"\n" << std::endl;


        auto start1 = std::chrono::high_resolution_clock::now();

        Vector<int> vec_1 = { 1, 3, 5, 7 };
        Vector<int> vec_2 = { 1, 3, 8, 7 };
        std::cout << vec_1[3] << "\n";
        vec_1 = vec_2;
        std::cout << "сложение Vector:  " << vec_1 << " + " << vec_2 << " = " << vec_1 + vec_2 << std::endl;
        std::cout << "вычитание Vector:  " << vec_1 << " - " << vec_2 << " = " << vec_1 - vec_2 << std::endl;
        std::cout << "умножение Vector:  " << vec_1 << " * " << vec_2 << " = " << vec_1 * vec_2 << std::endl;

        std::cout << "Matrix\n\n";

        Matrix<int, 4, 4> A =
        {
        {1, 2, 3, 7},
        {4, 5, 6, 8},
        {1, 2, 3, 6},
        {4, 5, 6, 3}
        };
        Matrix<int, 4, 4> B =
        {
        {1, 2, 3, 4},
        {4, 5, 6, 4},
        {1, 2, 3, 66},
        {4, 5, 6, 7}
        };
        std::cout << A * B;

        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);
       

        std::cout << "\n\n";

        auto start2 = std::chrono::high_resolution_clock::now();

        AVX_Vector<int> vec_11 = { 1, 3, 5, 7 };
        AVX_Vector<int> vec_22 = { 1, 3, 8, 7 };
        std::cout << vec_11[3] << "\n";
        vec_11 = vec_22;
        std::cout << "сложение AVX_Vector:  " << vec_11 << " + " << vec_22 << " = " << vec_11 + vec_22 << std::endl;
        std::cout << "вычитание AVX_Vector:  " << vec_11 << " - " << vec_22 << " = " << vec_11 - vec_22 << std::endl;
        std::cout << "умножение AVX_Vector:  " << vec_11 << " * " << vec_22 << " = " << vec_11 * vec_22 << std::endl;

        std::cout << "AVX_Matrix\n\n";

        AVX_Matrix<int, 4, 4> AA =
        {
        {1, 2, 3, 7},
        {4, 5, 6, 8},
        {1, 2, 3, 6},
        {4, 5, 6, 3}
        };
        AVX_Matrix<int, 4, 4> BB =
        {
        {1, 2, 3, 4},
        {4, 5, 6, 4},
        {1, 2, 3, 66},
        {4, 5, 6, 7}
        };
        std::cout << AA * BB;

        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);

        std::cout << "\n\n\n";


        std::cout << "Обычные классы: " << duration1.count() << "\n";
        std::cout << "AVX-классы: " << duration2.count() << "\n";
       
       
        

    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}