#include "dev_func.cpp"

#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<double> a(n);
    for (int i = 0; i < n; i++) { cin >> a[i]; }
    double ans = dev_func::find_median_BFPRT(a);
    cout << ans << "\n";
    return 0;
}