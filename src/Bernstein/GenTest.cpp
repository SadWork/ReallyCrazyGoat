#include <bits/stdc++.h>
using namespace std;

double f(const vector<double> &x)
{
    return -x[0] * x[0] - x[1] * x[1] + 1;
}

enum
{
    DEFAULT_TEST_NUMBER_POINTS = 5,
    CORRECT_NUMBER_POINTS = 1000,
};
int main(int argc, char *argv[])
{
    freopen("~test.txt", "w", stdout);
    ios::sync_with_stdio(false);

    constexpr int dimenstion = 2;
    int n = DEFAULT_TEST_NUMBER_POINTS;

    if (argc >= 2)
    {
        string s = argv[1];
        n = stoi(s);
    }

    random_device randD;
    uniform_real_distribution<double> dist(0., 1.);
    mt19937 gen(randD());

    vector<double> x(dimenstion);
    cout << dimenstion << " " << n << "\n";

    for (int i = 0; i < n; i++)
    {
        for (auto &j : x)
        {
            j = dist(gen);
            cout << j << " ";
        }
        cout << f(x) << "\n";
    }

    ofstream output("~correct.txt");

    output << dimenstion << " " << CORRECT_NUMBER_POINTS << "\n";

    for (int i = 0; i < CORRECT_NUMBER_POINTS; i++)
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