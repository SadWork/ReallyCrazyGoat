#include <bits/stdc++.h>
using namespace std;

struct BasePoints
{
    vector<double> points; // координаты опорных точек
    vector<double> x; // подобранные коэффиценты после решения СЛАУ

    int dimensions;

    int size() { return points.size() / dimensions; }

    void insert(const vector<double> &coords, int start, int end) // Добавление опорной точки
    {
        if (end - start != dimensions)
        {
            dimensions = end - start;
            points.clear();
        }
        for (int i = start; i < end; i++)
        {
            points.push_back(coords[i]);
        }

        x.resize(x.size() + 1, 0.);
    }

    void build_series(const vector<double> &coords, int start1, int end1, vector<double> &series, int start2, int end2) //  Вычисление "мономов" по данным координатам
    {
        // Каждый моном - это растояние от текущей точки до добавленных опорных точек
        
        int size = end2 - start2;
        if ((size > this->size()) or (end1 - start1 != this->dimensions))
        {
            exit(1);
        }
        for (int i = 0; i < size; i++)
        {
            double distance2 = 0.;
            for (int j = start1; j < end1; j++)
            {
                double diff = coords[j] - points[i * dimensions + j - start1];
                distance2 += diff * diff;
            }

            series[start2 + i] = 1. / sqrt(distance2 + 1. / this->size());
        }
    }

    void build_series(const vector<double> &coords, vector<double> &series)
    {
        this->build_series(coords, 0, coords.size(), series, 0, series.size());
    }
};
