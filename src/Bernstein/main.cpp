#include <bits/stdc++.h>
using namespace std;

static constexpr double eps = 1e-3;                // минимум для функции ошибки
static constexpr double h_eps = 1e-5;              // минимум для точек
static constexpr double exp_eps = 1e-5;            // смещение для experiments_size
static constexpr double learning_rate_boost = 1e3; // добавка для медленно меняющихся параметров
auto start = std::chrono::high_resolution_clock::now();

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
        uniform_real_distribution<Number> dist(0.0, M_PI);

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
            values[i] = dist(gen) / M_PI; // приводим к отрезку [0;1]
        }

        experiments_size.resize(size);
        for (int i = 0; i < size; i++)
        {
            experiments_size[i] = 0;
        }
    }

    Number calc_p(int i, vector<Number> &p)
    {
        Number ln_p = 0;
        Number normalize_exp = fabs(experiments_size[i]) + values.size();

        for (int j = 0; j < points[i].size(); j++)
        {
            Number pj = std::max(p[j], static_cast<Number>(h_eps));
            Number qj = std::max(1 - pj, static_cast<Number>(h_eps));

            // нормализуем параметры points[i][j] и experiments_size[i],
            // чтобы гарантировать попадание в область определения lgamma и log
            Number normalize_point = (1 - cos(points[i][j])) / 2;

            Number success_cnt = normalize_point * normalize_exp;
            Number failure_cnt = (1 - normalize_point) * normalize_exp;

            ln_p += lgamma(normalize_exp + 1);
            ln_p -= lgamma(success_cnt + 1);
            ln_p -= lgamma(failure_cnt + 1);
            ln_p += success_cnt * log(pj);
            ln_p += failure_cnt * log(qj);
        }
        return exp(ln_p);
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
            Number prob = calc_p(i, p);
            sum_p += prob;
            math_exp += values[i] * prob;
        }
        return math_exp / sum_p;
    }

    Number get_grad(vector<Number> &grad, vector<Number> &p)
    {
        // вычислает градиент **в точке**, сохраняя его в вектор grad. Возвращает значение функции в точке
        // x_i, y_i, f(x_i, y_i), experiments_size[i] для i = 1 .. N
        // порядок: x_1, y_1, ..., x_N, y_N, f(x_1, y_1), ..., f(x_N, y_N), n_1, ..., n_N

        // вычисляем "вероятности" каждого из слагаемых, их сумму и квадрат их суммы
        vector<Number> P(values.size());
        Number sum_P = 0, sum_P2;

        for (int i = 0; i < P.size(); ++i)
        {
            Number prob = calc_p(i, p);
            P[i] = prob;
            sum_P += prob;
        }
        sum_P2 = sum_P * sum_P;

        int grad_index = 0;

        // производные по x_i и y_i
        for (int i = 0; i < points.size(); i++)
        {
            // заранее считаем коэффициенты, не меняющиеся для всех j
            Number normalize_exp = fabs(experiments_size[i]) + values.size();
            vector<Number> ln_pk(points[i].size(), 0);
            for (int k = 0; k < points[i].size(); k++) {
                Number pk = std::max(p[k], static_cast<Number>(h_eps));
                Number qk = std::max(1 - pk, static_cast<Number>(h_eps));
                Number normalize_point = (1 - cos(points[i][k])) / 2;
                Number success_cnt = normalize_point * normalize_exp;
                Number failure_cnt = (1 - normalize_point) * normalize_exp;
                ln_pk[k] = lgamma(normalize_exp + 1);
                ln_pk[k] -= lgamma(success_cnt + 1);
                ln_pk[k] -= lgamma(failure_cnt + 1);
                ln_pk[k] += success_cnt * log(pk);
                ln_pk[k] += failure_cnt * log(qk);
            }

            for (int j = 0; j < points[i].size(); j++)
            {
                // производная для j-й координаты i-й точки
                Number original_value = points[i][j];
                points[i][j] += h_eps;

                // > start of `calc_p(i, p)`
                
                // вклад изменённой координаты:
                Number pj = std::max(p[j], static_cast<Number>(h_eps));
                Number qj = std::max(1 - pj, static_cast<Number>(h_eps));
                Number normalize_point = (1 - cos(points[i][j])) / 2;
                Number success_cnt = normalize_point * normalize_exp;
                Number failure_cnt = (1 - normalize_point) * normalize_exp;
                Number ln_p = lgamma(normalize_exp + 1);
                ln_p -= lgamma(success_cnt + 1);
                ln_p -= lgamma(failure_cnt + 1);
                ln_p += success_cnt * log(pj);
                ln_p += failure_cnt * log(qj);

                // вклад остальных координат:
                for (int k = 0; k < points[i].size(); k++)
                {
                    if (k == j) continue;
                    ln_p += ln_pk[k];
                }
                Number error_plus = exp(ln_p);

                // > end of `calc_p(i, p)`

                // восстанавливаем значение
                points[i][j] = original_value;

                Number deriv = (error_plus - P[i]) / h_eps;
                grad[grad_index++] = deriv * values[i] * (sum_P - P[i]) / sum_P2;
            }
        }

        // производные по f(x_i, y_i)
        for (int i = 0; i < values.size(); i++)
        {
            grad[grad_index++] = P[i] / sum_P;
        }

        // производные по n_i
        for (int i = 0; i < experiments_size.size(); i++)
        {
            Number original_value = experiments_size[i];

            experiments_size[i] += h_eps;
            Number error_plus = calc_p(i, p); // дорогие вычисления, видимо, сократить не судьба
            experiments_size[i] = original_value;

            Number deriv = (error_plus - P[i]) / h_eps;
            grad[grad_index++] = deriv * values[i] * (sum_P - P[i]) / sum_P2;
        }

        Number math_exp = 0;
        for (int i = 0; i < values.size(); ++i)
        {
            math_exp += P[i] * values[i];
        }

        return math_exp / sum_P;
    }

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
Number get_grad(vector<Number> &grad, BernsteinPolinom<Number> &bp, Data<Number> &data)
{
    fill(grad.begin(), grad.end(), 0);

    Number error = 0;
    vector<Number> grad_in_point(4 * bp.points.size());
    for (int i = 0; i < data.values.size(); ++i)
    {
        Number delta = bp.get_grad(grad_in_point, data.points[i]) - data.values[i];
        error += delta * delta;

        for (int j = 0; j < grad.size(); ++j)
        {
            grad[j] += 2 * delta * grad_in_point[j];
        }
    }

    return error;
}

template <class Number>
void accelerated_gradient_descent(BernsteinPolinom<Number> &bp, Data<Number> &data, int steps)
{
    // Шаг обучения (learning rate)
    Number learning_rate = 1e-5;
    // Коэффициент инерции (momentum)
    Number momentum_factor = 0.8;

    int dimension = bp.points[0].size();
    // Вектор градиентов
    vector<Number> grad((dimension + 2) * bp.points.size()); // dimension * bp.size + bp.values.size + bp.exp.size
    // Вектор для хранения предыдущего обновления (для momentum)
    vector<Number> prev_update(grad.size(), 0.0);
    // Вектор для хранения текущего обновления
    vector<Number> current_update(grad.size());

    for (int step = 0; step < steps; step++)
    {
        // Сохраняем текущие параметры
        BernsteinPolinom<Number> bp_lookahead = bp;

        // Применяем предыдущее обновление для "взгляда вперед"
        int update_index = 0;
        for (int i = 0; i < bp_lookahead.points.size(); i++)
        {
            for (int j = 0; j < bp_lookahead.points[i].size(); j++)
            {
                bp_lookahead.points[i][j] -= momentum_factor * prev_update[update_index++];
            }
        }
        for (int i = 0; i < bp_lookahead.values.size(); i++)
        {
            bp_lookahead.values[i] -= momentum_factor * prev_update[update_index++];
        }
        for (int i = 0; i < bp_lookahead.experiments_size.size(); i++)
        {
            bp_lookahead.experiments_size[i] -= momentum_factor * prev_update[update_index++];
        }

        // Вычисляем градиент в "предсказанной" точке
        Number error = get_grad(grad, bp_lookahead, data);

        // Вычисляем обновление с учетом momentum
        int grad_index = 0;
        for (int i = 0; i < bp.points.size(); i++)
        {
            for (int j = 0; j < bp.points[i].size(); j++)
            {
                current_update[grad_index] = momentum_factor * prev_update[grad_index] + learning_rate * grad[grad_index];
                bp.points[i][j] -= current_update[grad_index];
                grad_index++;
            }
        }
        for (int i = 0; i < bp.values.size(); i++)
        {
            current_update[grad_index] = momentum_factor * prev_update[grad_index] + learning_rate * grad[grad_index];
            bp.values[i] -= current_update[grad_index];
            grad_index++;
        }
        for (int i = 0; i < bp.experiments_size.size(); i++)
        {
            current_update[grad_index] = momentum_factor * prev_update[grad_index] + learning_rate * learning_rate * grad[grad_index];
            bp.experiments_size[i] -= current_update[grad_index];
            grad_index++;
        }

        // Обновляем предыдущее обновление
        prev_update = current_update;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        cout << "Шаг: " << step + 1 << ", Ошибка: " << error << endl;

        // Останавливаемся, если ошибка достаточно мала
        if (error < eps)
        {
            cout << "Ускоренный градиентный спуск завершён: ошибка достигла порогового значения." << endl;
            break;
        }
    }
}

template <class Number>
void gradient_descent(BernsteinPolinom<Number> &bp, Data<Number> &data, int steps)
{
    // Шаг обучения (learning rate)
    Number learning_rate = 0.005;

    // Вектор градиентов
    int dimension = bp.points[0].size();
    vector<Number> grad((dimension + 2) * bp.points.size());

    for (int step = 0; step < steps; step++)
    {
        // Вычисляем градиент
        Number error = get_grad(grad, bp, data);

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

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        cout << "Шаг: " << step + 1 << ", Ошибка: " << error << endl;

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

    auto start = std::chrono::high_resolution_clock::now();
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