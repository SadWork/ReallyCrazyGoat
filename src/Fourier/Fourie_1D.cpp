#include <bits/stdc++.h>
using namespace std;

constexpr double EPS = 1e-6;
constexpr double EPS_K = 1e-4;
constexpr double L = 0., R = 1.;

double func(double x)
{
    return x*x*x*x;
}

double integral(const vector<double> &x, function<double(double)> f)
{

    double M = f(x[0]);
    double m = M;

    for(int i=1; i<x.size(); i++)
    {
        double res = f(x[i]);
        M = max(res, M);
        m = min(res, m);
    }

    double ans = 0.;
    M -= m;

    if(abs(M) > EPS){
        for(int i=0; i<x.size(); i++)
        {
            ans += (f(x[i])-m)/M;
        }
    }else{
        ans = 1.;
    }

    double V = (R-L) * M;

    ans = ans * V / x.size();
    ans += m * (R - L);

    return ans;
}

int main()
{
    
    random_device rd;
    mt19937 g(rd());
    
    uniform_real_distribution<double> dist(L, R);
    
    int n, series;
    cin>>n >> series;

    vector<double> x(n);
    
    for(int i=0; i<n; i++)
    {
        x[i] = dist(g);
    }

    cout<<"\n\nf(x) = ";
    cout<<integral(x, func)<<"+";

    for(int i=1; i<series; i++)
    {
        double k = 2*M_PI*((i+1)/2);
        if(i%2)
        {
            auto cfunc = [&](double x) {
                return func(x) * sin(k*x); 
            };
            k = integral(x, cfunc) * 2;

            if(abs(k) > EPS_K)
            cout<<k<<"*sin("<<(i+1)/2<<"x) + ";
        }
        else
        {
            auto cfunc = [&](double x) {
                return func(x) * cos(k*x); 
            };
            k = integral(x, cfunc) * 2;

            if(abs(k) > EPS_K)
            cout<<k<<"*cos("<<(i+1)/2<<"x) + ";
        }
    }

    return 0;
}