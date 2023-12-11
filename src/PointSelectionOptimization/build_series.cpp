#include <bits/stdc++.h>

#define DEBUG0

#ifdef DEBUG
#define DEBUG_ONLY(code) code
#else
#define DEBUG_ONLY(code)
#endif

using namespace std;

/*
 * ВНИМАНИЕ ТУТ СЛОЖНА! или не очень. Если не понятно как работает, включите
 * #define DEBUG - это активирует строчки добавляющие в массив строк текстовое
 * описание работы. Можно во время дебага или просто запуска отследить как строится
 * серия коэффициентов - сравнить, посчитав ручками, при желании
 *
 * Функция должна получать вектор координат и требуемую длину ряда series.size()
 * В результате работы возвращаются значения ряда, записываются в массив
*/
 
void build_series(const vector<double> &coords, vector<double> &series);
void build_series(const vector<double> &coords, int start, int end, vector<double> &series, int start2, int end2);

void build_series_p(const vector<double> &coords, vector<double> &series);
void build_series_p(const vector<double> &coords, int start, int end, vector<double> &series, int start2, int end2);

void build_series_f(const vector<double> &coords, vector<double> &series);
void build_series_f(const vector<double> &coords, int start, int end, vector<double> &series, int start2, int end2);

void build_series(const vector<double> &coords, vector<double> &series)
{
    build_series(coords, 0, coords.size(), series, 0, series.size());
}
void build_series_p(const vector<double> &coords, vector<double> &series)
{
    build_series_p(coords, 0, coords.size(), series, 0, series.size());
}
void build_series_f(const vector<double> &coords, vector<double> &series)
{
    build_series_f(coords, 0, coords.size(), series, 0, series.size());
}

void build_series(const vector<double> &coords, int start1, int end1, vector<double> &series, int start2, int end2)
{
    int dimension = end1 - start1;
    int len = end2 - start2;
    int max_k = ceil((pow(len, 1. / dimension) - 1) / 2);
    double STEP = log2(len);

    series[start2] = 1.;

    DEBUG_ONLY(vector<string> debug_str);
    DEBUG_ONLY(debug_str.push_back("1"));

    int written_size = 1;
    int io = start2 + 1;

    for (int i = 0; i < dimension; i++)
    {
        for (int k = 1; k <= max_k; k++)
        {
            for (int j = 0; j < written_size; j++)
            {
                series[io++] = cos(coords[i + start1] * k * STEP) * series[j + start2];
                DEBUG_ONLY(debug_str.push_back("cos(" + to_string(k * STEP) + "*x_" + to_string(i) + ")*" + debug_str[j]));
                if (io == end2)
                {
                    goto EXIT;
                }
            }
            for (int j = 0; j < written_size; j++)
            {
                series[io++] = sin(coords[i + start1] * k * STEP) * series[j + start2];
                DEBUG_ONLY(debug_str.push_back("sin(" + to_string(k * STEP) + "*x_" + to_string(i) + ")*" + debug_str[j]));
                if (io == end2)
                {
                    goto EXIT;
                }
            }
        }
        written_size = io;
    }

EXIT:
#ifdef DEBUG
    for (auto s : debug_str)
    {
        cout << s << " ";
    }
    cout << "\n";
#endif
    return;
}

void build_series_p(const vector<double> &cords, int start1, int end1, vector<double> &series, int start2, int end2)
{
    int dimension = end1 - start1;
    int len = end2 - start2;
    int max_k = ceil(pow(len, 1. / dimension) - 1);

    series[start2] = 1.;

    DEBUG_ONLY(vector<string> debug_str);
    DEBUG_ONLY(debug_str.push_back("1"));

    int written_size = 1;
    int io = start2 + 1;

    for (int i = 0; i < dimension; i++)
    {
        double mul = cords[i + start1];
        for (int k = 1; k <= max_k; k++)
        {

            for (int j = 0; j < written_size; j++)
            {
                series[io++] = mul * series[start2 + j];
                DEBUG_ONLY(debug_str.push_back("x_" + to_string(i) + "^" + to_string(k) + "*" + debug_str[j]));
                if (io == end2)
                {
                    goto EXIT;
                }
            }

            mul *= cords[i + start1];
        }
        written_size = io;
    }

EXIT:
#ifdef DEBUG
    for (auto s : debug_str)
    {
        cout << s << " ";
    }
    cout << "\n";
#endif
    return;
}

void build_series_f(const vector<double> &coords, int start1, int end1, vector<double> &series, int start2, int end2)
{
    int dimension = end1 - start1;
    int len = end2 - start2;
    double STEP = 1.;
    int max_k = ceil(double(len - 1) / (dimension * 2));

    series[start2] = 1.;

    DEBUG_ONLY(vector<string> debug_str);
    DEBUG_ONLY(debug_str.push_back("1"));

    int io = start2 + 1;

    for (int i = 0; i < dimension; i++)
    {
        double tmp = coords[i + start1] * STEP;
        for (int k = 1; k <= max_k; k++)
        {
            series[io++] = cos(tmp * k);
            DEBUG_ONLY(debug_str.push_back("cos(" + to_string(k * STEP) + " * x" + to_string(i) + ")"));
            if(io == end2)
            {
                goto EXIT;   
            }
            series[io++] = sin(tmp * k);
            DEBUG_ONLY(debug_str.push_back("sin(" + to_string(k * STEP) + " * x" + to_string(i) + ")"));
            if(io == end2)
            {
                goto EXIT;   
            }
        }

    }

EXIT:
#ifdef DEBUG
    for (auto s : debug_str)
    {
        cout << s << " ";
    }
    cout << "\n";
#endif
    return;
}