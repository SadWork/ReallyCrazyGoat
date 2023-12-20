#pragma once
#include <bits/stdc++.h>

using namespace std;
using Real = double;

namespace dev_func
{
    Real find_order_statistics(vector<Real> &array, unsigned k, unsigned start, unsigned end);

    Real find_median_BFPRT(vector<Real> &array);

    void selection_sort(vector<Real> &array, unsigned start, unsigned end);
    unsigned partition(vector<Real> &array, Real x, unsigned start, unsigned end);

    Real find_median_BFPRT(vector<Real> &array) { return dev_func::find_order_statistics(array, array.size() / 2, 0, array.size()); }

    Real find_order_statistics(vector<Real> &array, unsigned k, unsigned start, unsigned end)
    {
        constexpr unsigned GROUP_SIZE = 5;

        unsigned num_groups = 0;

        if (end - start <= 5)
        {
            dev_func::selection_sort(array, start, end);
            return array[k];
        }

        for (unsigned i = start; i + 5 <= end; i += 5)
        {
            dev_func::selection_sort(array, i, i + 5);
            swap(array[num_groups + start], array[i + 2]);
            num_groups++;
        }

        Real x = dev_func::find_order_statistics(array, start + num_groups / 2, start, start + num_groups);

        unsigned l = dev_func::partition(array, x, start, end);
        unsigned r = l + 1;

        while ((r < end) && (array[r]) == x) { r++; }
        if (k <= l)
        {
            return dev_func::find_order_statistics(array, k, start, l + 1);
        }
        if (k >= r)
        {
            return dev_func::find_order_statistics(array, k, r, end);
        }
        return array[k];
    }

    void selection_sort(vector<Real> &array, unsigned start, unsigned end)
    {
        for (unsigned i = start; i < end; i++)
        {
            for (unsigned j = i + 1; j < end; j++)
            {
                if (array[i] > array[j])
                {
                    swap(array[i], array[j]);
                }
            }
        }
    }

    unsigned partition(vector<Real> &array, Real x, unsigned start, unsigned end)
    {
        unsigned l = start, r = end - 1;
        while (r > l)
        {
            while ((array[r] >= x) && (r > l)) { r--; }
            while ((array[l] < x) && (r > l)) { l++; }
            swap(array[l], array[r]);
        }
        return l; //(array[l] < x) ? l : (l - 1);
    }

    // Генерирует по порядку все выборы m элементов из множества |1, n|
    template <typename Integer> bool NextSetNoRepeat(vector<Integer> &a, int n, int m)
    {
        int k = m;
        for (int i = k - 1; i >= 0; --i)
            if (a[i] < n - k + i + 1)
            {
                ++a[i];
                for (int j = i + 1; j < k; ++j) a[j] = a[j - 1] + 1;
                return true;
            }
        return false;
    }

    // Генерирует по порядку все выборы с повторениями m элементов из множества |1, n|
    template <typename Integer> bool NextSet(vector<Integer> &a, int n, int m)
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

    // Генерирует следующую по порядку двоичную маску (прибавляет 1 к n битовому числу)
    bool NextMask(vector<bool> &a, int n)
    {
        int j = 0;
        while (a[j] && (j < n))
        {
            a[j] = 0;
            j++;
        }
        if (j == n)
        {
            return false;
        }
        a[j] = 1;
        return true;
    }
};