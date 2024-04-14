#include <bits/stdc++.h>
using namespace std;

double f(double x) { return pow(x * x * x + x - 2, 2); }
double second_derivative(double x) // error = O(eps^4)
{
    double eps = 1e-3;
    double f1 = f(x), f2 = f(x + eps), f3 = f(x - eps);
    double second_der = (f2 + f3 - 2 * f1) / eps / eps;
    return second_der;
}
double first_derivative(double x) // error = O(eps^3)
{
    double eps = 1e-3;
    double f2 = f(x + eps), f3 = f(x - eps);
    double first_der = (f2 - f3) / (2 * eps);
    return first_der;
}
int main()
{
    constexpr double C   = 10;
    constexpr double eps = 1e-9;
    double x;
    int steps;
    cin >> x >> steps;
    double x1, x2;
    x1 = x2 = x;
    for (int i = 0; i < steps; ++i)
    {
        double der22 = second_derivative(x2);
        double der12 = first_derivative(x2);
        double der1  = first_derivative(x1);

        double delta1 = min(1., abs(der1) / C) + eps;
        double delta2 = min(1., abs(der12) / max(2 * abs(der22), 1.)) + eps;

        x1 = x1 + ((der1 > 0) ? -1 : 1) * delta1;
        x2 = x2 + ((der12 > 0) ? -1 : 1) * delta2;
    }
    cout << x1 << " " << x2 << "\n";
    cout << f(x1) << " " << f(x2) << "\n";
    return 0;
}
