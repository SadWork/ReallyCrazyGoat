#include <bits/stdc++.h>
using namespace std;

double func(double x)
{
    return -(x*x*x*x);
}

int main()
{
    constexpr double eps = 1e-6;
    random_device rd;
    mt19937 g(rd());
    double L = 0., R = 1.;
    uniform_real_distribution<double> dist(L, R);
    
    int n;
    cin>>n;

    vector<double> x(n);
    
    for(int i=0; i<n; i++)
    {
        x[i] = func(dist(g));
    }

    double M = x[0];
    double m = x[0];

    for(auto i:x)
    {
        if(i > M)
        {
            M = i;
        }
        else if(i < m)
        {
            m = i;
        }
    }

    double ans = 0.;
    M -= m;
    if(abs(M) > eps){
        for(int i=0; i<n; i++)
        {
            ans += (x[i]-m)/M;
        }
    }else{
        ans = 1.;
    }
    
    double V = (R-L) * M;
    ans = ans * V / n;
    ans+= m * (R-L);
    cout<<ans<<"\n";

    return 0;
}