#include "../dev_func.cpp"
#include "../func.cpp"
#include "build_series_m.cpp"
// #include "../build_series.cpp"

#include <bits/stdc++.h>
#define build_series build_series_p_stable
using namespace std;

constexpr double EPSILON = 1e-2;
void func(const vector<double> &points, vector<double> b, vector<double> &x, const int number_point, const int dimension)
{
    random_device randD;
    uniform_int_distribution<int> dist(0., number_point);
    mt19937 gen(randD());

    vector<int> where_point(number_point);
    iota(where_point.begin(), where_point.end(), 0);

    double median;
    {
        vector<double> copy_b = b;

        median = dev_func::find_median_BFPRT(copy_b);
    }

    int next_point = 0;
    {
        double diff = abs(b[next_point] - median);
        for (int i = 1; i < number_point; i++)
        {
            double new_diff = abs(median - b[i]);
            if (new_diff < diff)
            {
                next_point = i;
                diff       = new_diff;
            }
        }
        swap(where_point[0], where_point[next_point]);
        swap(b[0], b[next_point]);
    }

    int series_size            = 1;
    int point_in_approximation = 1;

    x.resize(series_size);
    vector<vector<double>> A(number_point, vector<double>(series_size));

    for (double diff = EPSILON + 1.; diff > EPSILON;)
    {
        for (int i = 0; i < point_in_approximation; i++)
        {
            A[i].resize(series_size);
            int index = where_point[i];
            build_series(points, index * dimension, (index + 1) * dimension, A[i], 0, series_size);
        }

        if (solveLinearSystemPivot(A, b, x, point_in_approximation, series_size) != -1)
        {
            if (point_in_approximation == number_point)
            {
                break;
            }

            diff            = 0;
            double rel_diff = 0;
            for (int i = point_in_approximation; i < number_point; i++)
            {
                A[i].resize(series_size);
                int index = where_point[i];
                build_series(points, index * dimension, (index + 1) * dimension, A[i], 0, series_size);

                double res = 0.;
                for (int j = 0; j < series_size; j++) { res += A[i][j] * x[j]; }

                double diff_i = abs(res - b[i]);
                diff          = max(diff, diff_i);
                diff_i /= abs(b[i]);

                if (diff_i > rel_diff)
                {
                    rel_diff   = diff_i;
                    next_point = i;
                }
            }

            swap(where_point[point_in_approximation], where_point[next_point]);
            swap(b[point_in_approximation], b[next_point]);

            point_in_approximation++;

            if (series_size < point_in_approximation)
            {
                series_size++;
            }
        }
        else
        {
            series_size++;
        }
        x.resize(series_size);
    }
}

void merge(vector<double> &a, vector<double> &b, double k)
{
    if (a.size() < b.size())
    {
        a.resize(b.size());
    }

    for (int i = 0; i < b.size(); i++) { a[i] += b[i] * k; }
}

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
        for (int j = 0; j < dimension; j++) { cin >> points[i * dimension + j]; }
        cin >> b[i];
    }
    vector<double> x;

    double k             = 1.;
    vector<double> itr_b = b;
    vector<double> new_x;
    double max_diff = 1.;
    for (int i = 0; max_diff > 1e-2; i++)
    {
        func(points, itr_b, new_x, number_point, dimension);

        merge(x, new_x, k);
        int series_size = x.size();

        new_x.resize(series_size);

        max_diff = 0.;
        for (int j = 0; j < number_point; j++)
        {
            build_series(points, j * dimension, (j + 1) * dimension, new_x, 0, series_size);

            double cur_res = 0.;
            for (int u = 0; u < series_size; u++) { cur_res += new_x[u] * x[u]; }

            max_diff = max(abs(b[j] - cur_res), max_diff);
            itr_b[j] = (b[j] - cur_res);
        }
        for (int j = 0; j < number_point; j++) { itr_b[j] *= 2 * EPSILON / max_diff; }
        k = max_diff / (2 * EPSILON);
        cout << "i: " << i << "\t" << max_diff << "\n";
    }

    // Вывод полученных коэффициентов
    for (auto i : x) { cout << i << " "; }
    cout << "\n";
    cout << "Размерность: " << x.size() << "\n";

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
    int series_size = x.size();
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
        for (int j = 0; j < series_size; j++) { ans += B[j] * x[j]; }
        output << ans << "\n";
    }

    output.close();
    cout << "Файл успешно записан." << endl;
    cout << " Размерность: " << x.size() << endl;

    return 0;
}
