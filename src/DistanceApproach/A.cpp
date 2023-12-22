#include "build_series.cpp"
#include "dev_func.cpp"
#include "distance.cpp"
#include "func.cpp"

#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) // Нужно переименовать названия файлов для более удобной и понятной координации между кодами
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
    int number_point, dimension = 2;

    int originalWidth, originalHeight;

    cin >> originalWidth >> originalHeight;
    number_point = originalWidth * originalHeight;

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

    vector<vector<double>> A(number_point, vector<double>(series_size));

    double EPSILON = 1e-2;
    for (double diff = EPSILON + 1.; diff > EPSILON;)
    {
        for (int i = 0; i < base.size(); i++)
        {
            A[i].resize(series_size);
            int index = where_point[i];
            base.build_series(points, index * dimension, (index + 1) * dimension, A[i], 0, series_size);
        }

        int cnt_itrs = solveLinearSystemItr(A, b, base.x, point_in_approximation, series_size, EPSILON);
        {
            if (point_in_approximation == number_point)
            {
                break;
            }

            diff = 0;

            A[0].resize(series_size);

            for (int i = point_in_approximation; i < number_point; i++)
            {
                int index = where_point[i];
                base.build_series(points, index * dimension, (index + 1) * dimension, A[0], 0, series_size);

                double res = 0.;

                for (int j = 0; j < series_size; j++)
                {
                    res += A[0][j] * base.x[j];
                }

                double diff_i = abs(res - b[i]);

                if (diff_i > diff)
                {
                    next_point = i;
                    diff       = diff_i;
                }
            }

            swap(where_point[point_in_approximation], where_point[next_point]);
            swap(b[point_in_approximation], b[next_point]);
        }

        {
            int index = where_point[point_in_approximation];
            base.insert(points, index * dimension, (index + 1) * dimension);
        }

        point_in_approximation++;
        series_size++;

        cout << setw(20) << left << "Curr Difference: " << setw(10) << right << diff << setw(20) << left << "\tPoints in approximation: " << setw(10)
             << right << point_in_approximation << setw(10) << left << "\tItrs: " << setw(5) << right << cnt_itrs << "\n";
    }

    // Вывод полученных коэффициентов
    for (auto i : base.x)
    {
        cout << i << " ";
    }
    cout << "\n";

    // Дальше просто генерируем много случайных точек

    const int newWidth  = 4 * originalWidth;  // Ширина изображения с увеличенным разрешением
    const int newHeight = 4 * originalHeight; // Высота изображения с увеличенным разрешением

    // Открываем файл для записи точек
    ofstream output(path_output);

    if (!output.is_open())
    {
        cout << "Не удалось открыть файл." << endl;
        return 0;
    }

    const int NUMBER_POINTS = newWidth * newHeight;
    output << newWidth << " " << newHeight << "\n";

    vector<double> new_points(dimension);
    vector<double> B(series_size);

    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            double normalized_j = (2.0 * static_cast<double>(j) / (newWidth)) - 1.0;
            double normalized_i = (2.0 * static_cast<double>(i) / (newHeight)) - 1.0;

            new_points[0] = normalized_j;
            new_points[1] = normalized_i;

            output << new_points[0] << " " << new_points[1] << " ";

            double ans = 0.;
            base.build_series(new_points, B);

            for (int j = 0; j < series_size; j++)
            {
                ans += B[j] * base.x[j];
            }
            output << ans << "\n";
        }
    }

    output.close();

    cout << "Файл успешно записан." << endl;
    cout << " Размерность: " << base.x.size() << endl;

    return 0;
}
