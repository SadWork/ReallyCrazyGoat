#include <bits/stdc++.h>
using namespace std;

double f(const vector<double> &x)
{
    double ans = 1 + sqrt(4 * x[0]) + sin(4 * x[1]);
    return ans;
}

int main(int argc, char *argv[])
{
    freopen("test.txt", "w", stdout);
    ios::sync_with_stdio(false);
    constexpr int dimenstion = 2;
    int n                    = 50;

    if (argc == 2)
    {
        string s = argv[1];
        n        = stoi(s);
    }

    random_device randD;
    uniform_real_distribution<double> dist(0., 1.);
    mt19937 gen(randD());

    vector<double> x(dimenstion);
    cout << dimenstion << " " << n << "\n";
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