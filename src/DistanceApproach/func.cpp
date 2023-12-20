#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename Real> int solveLinearSystemPivot(vector<vector<Real>> &A, vector<Real> b, vector<Real> &x, int n, int m)
{
    const Real EPSILON = 1e-9;

    for (int col = 0, row = 0; row < n && col < m; ++col)
    {
        int pivot = row;
        for (int i = row + 1; i < n; i++)
        {
            if (abs(A[pivot][col]) < abs(A[i][col]))
            {
                pivot = i;
            }
        }

        if (abs(A[pivot][col]) > EPSILON)
        {
            swap(A[pivot], A[row]);
            swap(b[pivot], b[row]);

            for (int i = row + 1; i < n; ++i)
            {
                Real c = A[i][col] / A[row][col];
                for (int j = col; j < m; ++j)
                {
                    A[i][j] -= c * A[row][j];
                }
                b[i] -= c * b[row];
            }
            ++row;
        }
    }

    for (int row = n - 1; row >= 0; --row)
    {
        int col = 0;
        while (col < m)
        {
            if (abs(A[row][col]) > EPSILON)
            {
                break;
            }
            col++;
        }

        if (col == m)
        {
            if (abs(b[row]) > EPSILON)
            {
                return -1;
            }
            continue;
        }

        for (int j = col + 1; j < m; j++)
        {
            b[row] -= A[row][j] * x[j];
        }
        x[col] = b[row] / A[row][col];
    }
    return 0;
}

template <typename Real> int solveLinearSystem(vector<Real> &A, vector<Real> b, vector<Real> &x, int n, int m)
{
    const Real EPSILON = 1e-10;

    // Прямой ход метода Гаусса
    for (int i = 0; i < n; ++i)
    {
        int pivot = -1;
        for (int j = 0; j < m; ++j)
        {
            if (abs(A[i * m + j]) > EPSILON)
            {
                pivot = j;
                break;
            }
        }

        if (pivot == -1)
        {
            if (abs(b[i]) > EPSILON)
            {
                return -1;
            }
            continue; // Пропускаем нулевые строки
        }

        Real factor = A[i * m + pivot];
        for (int j = 0; j < m; ++j)
        {
            A[i * m + j] /= factor;
        }
        b[i] /= factor;

        for (int k = 0; k < n; ++k)
        {
            if (k != i)
            {
                Real multiplier = A[k * m + pivot];
                for (int j = 0; j < m; ++j)
                {
                    A[k * m + j] -= multiplier * A[i * m + j];
                }
                b[k] -= multiplier * b[i];
            }
        }
    }

    // Обратный ход метода Гаусса
    for (int i = 0; i < n; ++i)
    {
        int pivot = -1;
        for (int j = 0; j < m; ++j)
        {
            if (abs(A[i * m + j]) > EPSILON)
            {
                pivot = j;
                break;
            }
        }

        if (pivot != -1)
        {
            x[pivot] = b[i];
        }
    }

    return 0;
}

template <typename Real>
int solveLinearSystemItr(std::vector<std::vector<Real>> &A, std::vector<Real> &b, std::vector<Real> &x, int n, int m, Real tolerance)
{
    // Проверка на корректность размеров матрицы
    if (n != m)
    {
        cerr << "Size A isn't correct: " << n << " " << m << "\n";
        exit(1);
        return -1; // Некорректные размеры
    }

    // Предварительное уменьшение до диагонального преобладания
    // for (int i = 0; i < n; ++i)
    // {
    //     Real max_off_diag = 0.0;
    //     for (int j = 0; j < n; ++j)
    //     {
    //         if (i != j)
    //         {
    //             max_off_diag = std::max(max_off_diag, std::abs(A[i][j]));
    //         }
    //     }
    //     Real scaling_factor = 1.0 + max_off_diag / std::abs(A[i][i]);
    //     for (int j = 0; j < n; ++j)
    //     {
    //         A[i][j] *= scaling_factor;
    //     }
    //     b[i] *= scaling_factor;
    // }

    // Инициализация переменных для итераций и установка максимального числа итераций
    int iter                 = 0;
    const int max_iterations = 2000;

    // Итерации метода Гаусса-Зейделя
    while (iter < max_iterations)
    {
        Real error = 0.0;
        for (int i = 0; i < n; ++i)
        {
            Real sum = 0.0;
            for (int j = 0; j < i; ++j)
            {
                sum += A[i][j] * x[j];
            }
            for (int j = i + 1; j < n; ++j)
            {
                sum += A[i][j] * x[j];
            }
            Real new_x = (b[i] - sum) / A[i][i];
            x[i]       = new_x;
        }

        for (int i = 0; i < n; i++)
        {
            Real sum = 0.;
            for (int j = 0; j < n; ++j)
            {
                sum += A[i][j] * x[j];
            }
            error = max(error, abs(sum - b[i]));
        }

        // Проверка на сходимость
        if (abs(error) < tolerance)
        {
            break;
        }
        ++iter;
    }

    // Проверка, достигнута ли сходимость
    if (iter >= max_iterations)
    {
        return -1; // Достигнуто максимальное число итераций
    }

    return iter;   // Количество выполненных итераций
}

template <typename Real> void multiplySqMatrix(vector<Real> &A, vector<Real> &B, vector<Real> &C, int n)
{
    fill(C.begin(), C.end(), Real(0));
    for (int i = 0; i < n; i++)
    {
        for (int s = 0; s < n; s++)
        {
            for (int j = 0; j < n; j++)
            {
                C[i * n + j] += A[i * n + s] * B[s * n + j];
            }
        }
    }
}
