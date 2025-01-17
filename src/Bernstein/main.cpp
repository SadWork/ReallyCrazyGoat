#include <bits/stdc++.h>
using namespace std;
static constexpr double eps = 1e-10;

template <class Number>
class Data
{
public:
    vector<vector<Number>> points;
    vector<Number> values;

private:
};

template <class Number>
class BernsteinPolinom
{
public:
    vector<vector<Number>> points;   // Координаты точек (x_i, y_i)
    vector<Number> values;           // Значения функции в точках f(x_i, y_i)
    vector<Number> experiments_size; // n_i

    BernsteinPolinom(int size, int dimension, mt19937 &gen)
    {
        uniform_real_distribution<Number> dist(0.0, 1.0);

        points.resize(size, vector<Number>(dimension));
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                points[i][j] = dist(gen);
            }
        }

        values.resize(size);
        for (int i = 0; i < size; i++)
        {
            values[i] = dist(gen);
        }

        static constexpr Number exp_offset = 1.0;
        experiments_size.resize(size);
        for (int i = 0; i < size; i++)
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
        for (int i = 0; i < points.size(); i++)
        {
            Number ln_p = 0;
            for (int j = 0; j < points[i].size(); j++)
            {
                Number pj = std::max(p[j], static_cast<Number>(eps));
                Number qj = std::max(1 - pj, static_cast<Number>(eps));

                Number success_cnt = points[i][j] * experiments_size[i];
                Number failure_cnt = (1 - points[i][j]) * experiments_size[i];

                ln_p += lgamma(experiments_size[i] + 1);
                ln_p -= lgamma(success_cnt + 1);
                ln_p -= lgamma(failure_cnt + 1);
                ln_p += success_cnt * log(pj);
                ln_p += failure_cnt * log(qj);
            }
            Number prob = exp(ln_p);
            sum_p += prob;
            math_exp += values[i] * prob;
        }
        return math_exp / sum_p;
    }

    void get_grad(vector<Number> &grad, Data<Number> &data, Number h = 1e-7)
    {
        grad.clear();
        grad.resize(4 * points.size()); // x_i, y_i, f(x_i, y_i), experiments_size[i] для i = 1 .. N
        // порядок: x_1, y_1, ..., x_N, y_N, f(x_1, y_1), ..., f(x_N, y_N), n_1, ..., n_N

        // производные по x_i и y_i
        int grad_index = 0;
        for (int i = 0; i < points.size(); i++)
        {
            for (int j = 0; j < points[i].size(); j++)
            {
                Number original_value = points[i][j];

                // производная для j-й координаты i-й точки
                points[i][j] += h;
                Number error_plus = TotalError(*this, data);
                points[i][j] -= 2 * h;
                Number error_minus = TotalError(*this, data);
                points[i][j] = original_value;

                grad[grad_index++] = (error_plus - error_minus) / (2 * h);
            }
        }

        // производные по f(x_i, y_i)
        for (int i = 0; i < values.size(); i++)
        {
            Number original_value = values[i];

            values[i] += h;
            Number error_plus = TotalError(*this, data);
            values[i] -= 2 * h;
            Number error_minus = TotalError(*this, data);
            values[i] = original_value;

            grad[grad_index++] = (error_plus - error_minus) / (2 * h);
        }

        // производные по n_i
        for (int i = 0; i < experiments_size.size(); i++)
        {
            Number original_value = experiments_size[i];

            experiments_size[i] += h;
            Number error_plus = TotalError(*this, data);
            experiments_size[i] -= 2 * h;
            Number error_minus = TotalError(*this, data);
            experiments_size[i] = original_value;

            grad[grad_index++] = (error_plus - error_minus) / (2 * h);
        }
    }

private:
};

template <class Number>
Number TotalError(BernsteinPolinom<Number> &bp, Data<Number> &data)
{
    Number res = 0;
    for (int i = 0; i < data.values.size(); i++)
    {
        Number delta = bp.calc(data.points[i]) - data.values[i];
        res += delta * delta;
    }
    return res;
}

template <class Number>
void gradient_descent(BernsteinPolinom<Number> &bp, Data<Number> &data, int steps)
{
    // Шаг обучения (learning rate)
    Number learning_rate = 0.01;

    // Вектор градиентов
    vector<Number> grad;

    for (int step = 0; step < steps; step++)
    {
        // Вычисляем градиент
        bp.get_grad(grad, data);

        // Обновляем точки (x_i, y_i)
        int grad_index = 0;
        for (int i = 0; i < bp.points.size(); i++)
        {
            for (int j = 0; j < bp.points[i].size(); j++)
            {
                bp.points[i][j] -= learning_rate * grad[grad_index++];
            }
        }

        // Обновляем значения функции f(x_i, y_i)
        for (int i = 0; i < bp.values.size(); i++)
        {
            bp.values[i] -= learning_rate * grad[grad_index++];
        }

        // Обновляем weights (experiments_size)
        for (int i = 0; i < bp.experiments_size.size(); i++)
        {
            bp.experiments_size[i] -= learning_rate * grad[grad_index++];
        }

        // Вычисляем текущую ошибку
        Number error = TotalError(bp, data);
        // cout << "Шаг: " << step + 1 << ", Ошибка: " << error << endl;

        // Останавливаемся, если ошибка достаточно мала
        if (error < eps)
        {
            cout << "Градиентный спуск завершён: ошибка достигла порогового значения." << endl;
            break;
        }
    }
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
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " <steps> <bernstein_size>\n";
        return 1;
    }

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

    for (int i = 0; i < data_size; i++)
    {
        for (auto &j : data.points[i])
        {
            cin >> j;
        }
        cin >> data.values[i];
    }

    // Тест функции градиента
    // vector<Real> grad;
    // bp.get_grad(grad, data);
    // cout << "Градиенты: \n";
    // for (int i = 0; i < grad.size(); i++)
    // {
    //     cout << "grad[" << i << "] = " << grad[i] << endl;
    // }

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

    // Тестовая проверка функции ошибки
    Real error = TotalError(bp, data);
    cout << "Ошибка: " << error << endl;

    return 0;
}
