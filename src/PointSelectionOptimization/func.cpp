#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename Real> int solveLinearSystemPivot(vector<vector<Real>> &A, vector<Real> b, vector<Real> &x, int n, int m)
{
    const Real EPSILON = 1e-9;

    for (int row = 0, col = 0; col < n && row < m; ++row)
    {
        int pivot = col;
        for (int i = col + 1; i < n; i++)
        {
            if (abs(A[pivot][row]) < abs(A[i][row]))
            {
                pivot = i;
            }
        }

        if (abs(A[pivot][row]) > EPSILON)
        {
            swap(A[pivot], A[col]);
            swap(b[pivot], b[col]);

            for (int i = col + 1; i < n; ++i)
            {
                Real c = A[i][row] / A[col][row];
                for (int j = row; j < m; ++j) { A[i][j] -= c * A[col][j]; }
                b[i] -= c * b[col];
            }
            ++col;
        }
    }

    for (int col = n - 1; col >= 0; --col)
    {
        int row = 0;
        while (row < m)
        {
            if (abs(A[col][row]) > EPSILON)
            {
                break;
            }
            row++;
        }

        if (row == m)
        {
            if (abs(b[col]) > EPSILON)
            {
                return -1;
            }
            continue;
        }

        for (int j = row + 1; j < m; j++) { b[col] -= A[col][j] * x[j]; }
        x[row] = b[col] / A[col][row];
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
        for (int j = 0; j < m; ++j) { A[i * m + j] /= factor; }
        b[i] /= factor;

        for (int k = 0; k < n; ++k)
        {
            if (k != i)
            {
                Real multiplier = A[k * m + pivot];
                for (int j = 0; j < m; ++j) { A[k * m + j] -= multiplier * A[i * m + j]; }
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

template <typename Real> void multiplySqMatrix(vector<Real> &A, vector<Real> &B, vector<Real> &C, int n)
{
    fill(C.begin(), C.end(), Real(0));
    for (int i = 0; i < n; i++)
    {
        for (int s = 0; s < n; s++)
        {
            for (int j = 0; j < n; j++) { C[i * n + j] += A[i * n + s] * B[s * n + j]; }
        }
    }
}

template <typename Real> bool NextSet(vector<Real> &a, int n, int m)
{
    int j = m - 1;
    while (a[j] == n && j >= 0) j--;
    if (j < 0)
        return false;
    if (a[j] >= n)
        j--;
    a[j]++;
    if (j == m - 1)
        return true;
    for (int k = j + 1; k < m; k++) a[k] = a[j];
    return true;
}
