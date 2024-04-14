#include <bits/stdc++.h>
using namespace std;

constexpr double L = 0., R = 1.;
int data_points;

struct func
{
    double k[3];
    static const int N = sizeof(k) / sizeof(k[0]);
    double calc(double x) { return k[0] * sin(k[1] * (x + k[2])); }
};

double f(double x)
{ // target
    return cos(x);
}

double calc(vector<func> &k, double x)
{
    double ans = 0;
    for (auto &i : k)
    {
        ans += i.calc(x);
    }
    return ans;
}
double loss(vector<func> &k)
{
    double ans = 0., step = (R - L) / data_points;
    for (int i = 0; i < data_points; i++)
    {
        double x = L + i * step;
        double r = f(x) - calc(k, x);
        ans += r * r;
    }
    return ans;
}

void second_derivative(vector<func> &x, vector<func> &d2x)
{
    double eps = 1e-3;
    for (int i = 0; i < x.size(); i++)
    {
        for (int j = 0; j < x[i].N; j++)
        {
            double f1 = loss(x);
            x[i].k[j] += eps;
            double f2 = loss(x);
            x[i].k[j] -= 2 * eps;
            double f3 = loss(x);
            x[i].k[j] += eps;
            d2x[i].k[j] = (f2 + f3 - 2 * f1) / eps / eps;
        }
    }
}
void first_derivative(vector<func> &x, vector<func> &dx)
{

    double eps = 1e-3;
    for (int i = 0; i < x.size(); i++)
    {
        for (int j = 0; j < x[i].N; j++)
        {
            x[i].k[j] += eps;
            double f2 = loss(x);
            x[i].k[j] -= 2 * eps;
            double f3 = loss(x);
            x[i].k[j] += eps;

            dx[i].k[j] = (f2 - f3) / (2 * eps);
        }
    }
}

int main()
{
    random_device randD;
    uniform_real_distribution<double> dist(-1., 1.);
    mt19937 gen(randD());

    constexpr double C   = 10;
    constexpr double eps = 1e-9;
    int steps, size;
    cin >> size >> steps >> data_points;

    vector<func> k(size);
    for (auto &i : k)
    {
        for (int u = 0; u < i.N; u++)
        {
            i.k[u] = dist(gen);
        }
    }

    vector<func> dk(size), d2k(size);
    cout << loss(k) << "\n";
    for (int i = 0; i < steps; i++)
    {
        second_derivative(k, d2k);
        first_derivative(k, dk);
        for (int j = 0; j < k.size(); j++)
        {
            for (int u = 0; u < k[j].N; u++)
            {
                double delta = min(0.5, abs(dk[j].k[u])) / max(2 * abs(d2k[j].k[u]), 1.) + eps;
                k[j].k[u] += ((dk[j].k[u] > 0) ? -1 : 1) * delta;
            }
        }
    }
    cout << loss(k) << "\n";
    for (auto &i : k)
    {
        printf("(%lf)(sin(%lf*(x+%lf)))+", i.k[0], i.k[1], i.k[2]);
    }
    cout << "\n";

    return 0;
}
