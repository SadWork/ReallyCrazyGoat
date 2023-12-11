#include <bits/stdc++.h>
#include "build_series.cpp"
#include "func.cpp"
#define build_series build_series_p
using namespace std;

int main(int argc, char *argv[])
{
    // Получаем файл на запись приближенных точек
    std::string path_output;
    if (argc != 2)
    {
        path_output = "approximation.txt";
    }
    else
    {
        path_output = argv[1];
    }
    cout << path_output << "\n";
    
    // Читаем количество переменных, количество точек
    int number_point, dimension;
    cin >> dimension >> number_point;
    
    // Читаем все точки в массив
    vector<double> points(dimension * number_point);
    vector<double> b(number_point);
    for (int i = 0; i < number_point; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            cin >> points[i * dimension + j];
        }
        cin >> b[i];
    }
    
    
    int series_size = 1;
    vector<double> x(series_size);
    vector<vector<double>> A(number_point, vector<double>(1, 1.));

    while (solveLinearSystemPivot(A, b, x, number_point, series_size) == -1)
    {
        cout << x.size() << "\n";
        series_size = min(series_size * 2, number_point);

        for (int i = 0; i < number_point; i++)
        {
            A[i].resize(series_size);
        }
        x.resize(series_size);

        for (int i = 0; i < number_point; i++)
        {
            /* Из известных значений координат получаем серию вычисленных коэффициентов
            *
            * build_series_p: коэффициенты - это полиномы от x0 ... xn
            * build_series: коэффициенты - это линейная комбинация sin(kj * xj) * sin(ki * xi)
            * или cos(kj*xj) * sin (ki * xi), или sin()cos(), или cos()cos()
            * build_series_f: т.к. произведение тригонометрических функций можно представить 
            * в виде суммы, то series_f - это линейная комбинация sin(ki * xi), cos(ki * xi)
            *
            */
            build_series(points, i * dimension, (i + 1) * dimension, A[i], 0, series_size);
        }
    }
    
    // Вывод полученных коэффициентов
    for (auto i : x)
    {
        cout << i << " ";
    }
    cout << "\n";


    // Дальше просто генерируем много случайных точек
    
    ofstream output(path_output);
    if (!output.is_open())
    {
        cout << "Не удалось открыть файл." << endl;
        return 0;
    }

    random_device randD;
    uniform_real_distribution<double> dist(0., 1.);
    mt19937 gen(randD());

    int NUMBER_POINTS = 500; // number_point;
    output << NUMBER_POINTS << " " << dimension << "\n";

    vector<double> new_points(dimension);
    vector<double> B(series_size);

    for (int i = 0; i < NUMBER_POINTS; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            new_points[j] = dist(gen);
            output << new_points[j] << " ";
        }

        double tmp;

        build_series(new_points, B);
        double ans = 0;
        for (int j = 0; j < series_size; j++)
        {
            ans += B[j] * x[j];
        }
        output << ans << "\n";
    }

    output.close();
    cout << "Файл успешно записан." << endl;
    cout << " Размерность: " << x.size() << endl;

    return 0;
}
