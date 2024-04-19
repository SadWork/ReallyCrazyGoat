#include <bits/stdc++.h>
using namespace std;

double f(const vector<double> &x)
{
    // double x0 = 0.5, y0 = 0.5, r = 0.4;
    // if((x0 - x[0])*(x0 - x[0]) + (y0 - x[1])* (y0 - x[1]) <= r * r)
    // {
    //     return 1.;
    // }
    // return 0.;
    return -x[0]*x[0]-x[1]*x[1] + 1; 
}

int main(int argc, char *argv[])
{
    freopen("test.txt", "w", stdout);
    ios::sync_with_stdio(false);
    constexpr int dimenstion = 2;
    int fourier_size = 20;
    int n = 50;

    if (argc == 2)
    {
        string s = argv[1];
        n = stoi(s);
    }

    random_device randD;
    uniform_real_distribution<double> dist(0., 1.);
    mt19937 gen(randD());

    vector<double> x(dimenstion);
    cout << dimenstion << " " << n << " " << fourier_size
         << "\n";
    cout << setprecision(12);

    for (int i = 0; i < n; i++)
    {
        for (auto &j : x)
        {
            j = dist(gen);
            cout << j << " ";
        }
        cout << f(x) << "\n";
    }

    ofstream output("correct.txt");

    n = 1000;
    output << dimenstion << " " << n << "\n";

    for (int i = 0; i < n; i++)
    {
        for (auto &j : x)
        {
            j = dist(gen);
            output << j << " ";
        }
        output << f(x) << "\n";
    }
    output.close();
    return 0;
}