#include <bits/stdc++.h>
using namespace std;

static constexpr double eps = 1e-10;

template <class Number>
class BernsteinPolinom
{
public:
    vector<vector<Number>> points;
    vector<Number> values;
    vector<Number> experiments_size;

    BernsteinPolinom(int size, int dimension, mt19937 &gen)
    {
        uniform_real_distribution<Number> dist(0.0, 1.0);

        points.resize(size, vector<Number>(dimension));
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                points[i][j] = dist(gen);
            }
        }

        values.resize(size);
        for (int i = 0; i < size; ++i)
        {
            values[i] = dist(gen);
        }

        static constexpr Number exp_offset = 1.0;
        experiments_size.resize(size);
        for (int i = 0; i < size; ++i)
        {
            experiments_size[i] = dist(gen) + exp_offset;
        }
    }

    Number calc(vector<Number> &p)
    {
        if (p.size() != points[0].size())
        {
            fprintf(stderr, "p.size() != point.size()\n");
            exit(1);
        }

        Number math_exp = 0.;
        Number sum_p = 0.;
        for (int i = 0; i < points.size(); ++i)
        {
            Number ln_p = 0;
            for (int j = 0; j < points[i].size(); ++j)
            {
                Number pj = std::max(p[j], static_cast<Number>(eps));
                Number qj = std::max(1 - pj, static_cast<Number>(eps));

                Number succes_cnt = points[i][j] * experiments_size[i];
                Number failure_cnt = (1 - points[i][j]) * experiments_size[i];

                ln_p += lgammaf64(experiments_size[i] + 1);
                ln_p -= lgammaf64(succes_cnt + 1);
                ln_p -= lgammaf64(failure_cnt + 1);
                ln_p += succes_cnt * logf64(pj);
                ln_p += failure_cnt * logf64(qj);
            }
            Number prob = exp(ln_p);
            sum_p += prob;
            math_exp += values[i] * prob;
        }
        return math_exp / sum_p;
    }

    void get_grad(vector<Number> &g)
    {
        // TODO gradient calculation
    }

private:
};

template <class Number>
class Data
{
public:
    vector<vector<Number>> points;
    vector<Number> values;

private:
};

template <class Number>
Number TotalError(BernsteinPolinom<Number> &bp, Data<Number> &data)
{
    Number res = 0;
    for (int i = 0; i < data.values.size(); ++i)
    {
        Number delta = bp.calc(data.points[i]) - data.values[i];
        res += delta * delta;
    }
    return res;
}

template <class Number>
void gradient_descent(BernsteinPolinom<Number> &bp, Data<Number> &data, int steps)
{
}

enum
{
    ARG_STEPS = 1,
    ARG_BERNSTEIN_SIZE = 2,

    OUTPUT_NUMBER_POINTS = 1000,
};

using Real = double;
int main(const int argc, const char *argv[])
{
    int gradient_steps = strtol(argv[ARG_STEPS], NULL, 0);
    int bernstein_size = strtol(argv[ARG_BERNSTEIN_SIZE], NULL, 0);

    int dimensions, data_size, experiments_size;
    cin >> dimensions >> data_size;

    random_device randD;
    mt19937 gen(randD());

    BernsteinPolinom<Real> bp(bernstein_size, dimensions, gen);
    Data<Real> data;

    data.points.resize(data_size, vector<Real>(dimensions));
    data.values.resize(data_size);

    for (int i = 0; i < data_size; ++i)
    {
        for (auto &j : data.points[i])
        {
            cin >> j;
        }
        cin >> data.values[i];
    }

    gradient_descent(bp, data, gradient_steps);

    string output_path = "~approximation.txt";
    ofstream output(output_path);
    if (!output.is_open())
    {
        cout << "Failed to open output file\n";
        return 0;
    }

    uniform_real_distribution<Real> dist(0., 1.);

    output << OUTPUT_NUMBER_POINTS << " " << dimensions << "\n";

    vector<Real> new_point(dimensions);

    for (int i = 0; i < OUTPUT_NUMBER_POINTS; i++)
    {
        for (int j = 0; j < dimensions; j++)
        {
            new_point[j] = dist(gen);
            output << new_point[j] << " ";
        }

        output << bp.calc(new_point) << "\n";
    }

    output.close();
    cout << "Файл успешно записан." << endl;
    return 0;
}