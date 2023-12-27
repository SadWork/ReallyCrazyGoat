#include "build_series.cpp"
#include "dev_func.cpp"
#include "distance.cpp"
#include "func.cpp"

#include <bits/stdc++.h>
using namespace std;

constexpr double EPSILON = 5e-2;

void make_swaps(vector<pair<double, int>> &contenders)
{
    for (int i = 1; i < contenders.size(); i++)
    {
        if (contenders[i - 1].first > contenders[i].first)
        {
            swap(contenders[i - 1], contenders[i]);
        }
    }
}

double evolve(BasePoints &base, const vector<double> &points, vector<double> &b, vector<int> &where_point, int &cnt_itrs)
{
    static constexpr int CONTENDERS_SIZE = 50;
    static vector<vector<double>> A;
    static vector<int> point_where;

    int number_point = where_point.size();
    int series_size  = base.size();
    int dimensions   = base.dimensions;

    vector<pair<double, int>> contenders(min(CONTENDERS_SIZE + 1, int(where_point.size() - base.size())));

    double diff = 0.;

    A.resize(series_size);
    for (int i = 0; i < series_size; i++)
    {
        A[i].resize(series_size);

        int index = where_point[i];
        base.build_series(points, index * dimensions, (index + 1) * dimensions, A[i], 0, series_size);
    }

    cnt_itrs = solveLinearSystemItr(A, b, base.x, series_size, series_size, EPSILON);

    for (int i = series_size; i < series_size + contenders.size(); i++)
    {
        int index = where_point[i];
        base.build_series(points, index * dimensions, (index + 1) * dimensions, A[0], 0, series_size);

        double res = 0.;

        for (int j = 0; j < series_size; j++)
        {
            res += A[0][j] * base.x[j];
        }

        double diff_i = abs(res - b[i]);
        diff          = max(diff_i, diff);

        contenders[i - series_size] = {diff_i, index};
    }

    sort(contenders.begin(), contenders.end());

    for (int i = series_size + contenders.size(); i < number_point; i++)
    {
        int index = where_point[i];
        base.build_series(points, index * dimensions, (index + 1) * dimensions, A[0], 0, series_size);

        double res = 0.;

        for (int j = 0; j < series_size; j++)
        {
            res += A[0][j] * base.x[j];
        }

        double diff_i = abs(res - b[i]);
        diff          = max(diff_i, diff);

        contenders[0] = {diff_i, index};
        make_swaps(contenders);
    }

    if (diff >= EPSILON)
    {

        int start = 1;
        if (contenders.size() < CONTENDERS_SIZE + 1)
        {
            start = 0;
        }

        point_where.resize(number_point);
        for (int i = 0; i < number_point; i++)
        {
            point_where[where_point[i]] = i;
        }

        for (int i = start; i < contenders.size(); i++)
        {
            int index_data = contenders[i].second;    // начало и конец в массиве point
            int index_b    = point_where[index_data]; // в какой строке матрицы

            base.insert(points, index_data * dimensions, (index_data + 1) * dimensions);

            int tmp = where_point[series_size];

            where_point[series_size] = index_data; // В новую строку помещаем новые данные
            where_point[index_b] = tmp; // В старую строку записываем значение которое меняли ранее

            swap(b[index_b], b[series_size]);
            swap(point_where[index_data], point_where[tmp]);

            series_size++;
        }
    }

    return diff;
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
    cout << "Output path: " << path_output << "\n";

    // Читаем количество переменных, количество точек
    int number_point, dimension;
    cin >> dimension >> number_point;

    // Читаем все точки в массив
    vector<double> points(dimension * number_point);
    vector<int> where_point(number_point);

    vector<double> b(number_point);

    for (int i = 0; i < number_point; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            cin >> points[i * dimension + j];
        }
        cin >> b[i];
        where_point[i] = i;
    }

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

    BasePoints base;
    {
        int index = where_point[0];
        base.insert(points, index * dimension, (index + 1) * dimension);
    }

    double EPSILON = 5e-2;

    for (double diff = EPSILON + 1.; diff > EPSILON;)
    {
        series_size = base.size();
        int cnt_itrs;
        diff = evolve(base, points, b, where_point, cnt_itrs);
        cout << setw(20) << left << "Curr Difference: " << setw(10) << right << diff << setw(20) << left << "\tPoints in approximation: " << setw(10)
             << right << series_size << setw(10) << left << "\tItrs: " << setw(5) << right << cnt_itrs << "\n";
    }

    // Вывод полученных коэффициентов
    for (auto i : base.x)
    {
        cout << i << " ";
    }
    cout << "\n";

    // Дальше просто генерируем много случайных точек

    // Открываем файл для записи точек
    ofstream output(path_output);

    if (!output.is_open())
    {
        cout << "Не удалось открыть файл." << endl;
        return 0;
    }

    random_device randD;
    uniform_real_distribution<double> dist(0., 1.);
    mt19937 gen(randD());

    int NUMBER_POINTS = 1000; // number_point;
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

        base.build_series(new_points, B);
        double ans = 0;
        for (int j = 0; j < series_size; j++)
        {
            ans += B[j] * base.x[j];
        }
        output << ans << "\n";
    }

    output.close();
    cout << "Файл успешно записан." << endl;
    cout << " Размерность: " << base.x.size() << endl;

    return 0;
}
