#include <bits/stdc++.h>
#include "build_series.cpp"
#include "func.cpp"
#define build_series build_series
using namespace std;

int main(int argc, char *argv[])
{
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
    int number_point, dimension;
    cin >> dimension >> number_point;
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

    const int MAX_SIZE = 20;
    int number_groups = (number_point + MAX_SIZE - 1) / MAX_SIZE;
    vector<vector<double>> x(number_groups, vector<double>(MAX_SIZE));
    vector<vector<double>> A;

    for (int igroup = 0, io = 0; igroup < number_groups; igroup++)
    {
        while (A.size() < MAX_SIZE)
        {
            A.push_back(vector<double>(MAX_SIZE));

            build_series(points, io * dimension, (io + 1) * dimension, A[A.size() - 1], 0, MAX_SIZE);

            if (solveLinearSystemPivot(A, b, x[igroup], A.size(), MAX_SIZE) == -1)
            {
                A.pop_back();
            }
            io++;
            if (io == number_point)
            {
                break;
            }
        }
        if (io == number_point)
        {
            number_groups = igroup + 1;
            break;
        }
    }

    vector<double> ans(MAX_SIZE, 0.);
    for (int i = 0; i < number_groups; i++)
    {

        for (int j = 0; j < MAX_SIZE; j++)
        {
            ans[j] += x[i][j];
        }
    }

    for (auto &i : ans)
    {
        i /= number_groups;
    }

    for (auto i : ans)
    {
        cout << i << " ";
    }
    cout << "\n";

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
    output << dimension << " " <<NUMBER_POINTS << "\n";

    vector<double> new_points(dimension);
    vector<double> B(MAX_SIZE);

    for (int i = 0; i < NUMBER_POINTS; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            new_points[j] = dist(gen);
            // new_points[j] = points[i*dimension+j];
            // cin>>j;
            output << new_points[j] << " ";
        }

        double tmp;

        build_series(new_points, B);
        double res = 0;
        for (int j = 0; j < MAX_SIZE; j++)
        {
            res += B[j] * ans[j];
        }
        output << res << "\n";
    }

    output.close();
    cout << "Файл успешно записан." << endl;
    cout << " Количество групп: " << number_groups << endl;

    return 0;
}