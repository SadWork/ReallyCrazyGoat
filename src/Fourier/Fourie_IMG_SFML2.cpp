#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "dev_func.cpp"
using namespace std;
using ll = long long;

constexpr double L = 0., R = 1.;
constexpr double EPS_K = 1e-9;
constexpr double EPS = 1e-6;

struct FSeries
{
    double C;
    vector<function<double(const vector<double> &)>> terms;
    vector<bool> type;
    vector<double> k;

    double value_in_point(const vector<double> &point)
    {
        double ans = C;
        for (int i = 0; i < terms.size(); i++)
        {
            ans += k[i] * terms[i](point);
        }
        return ans;
    }
};

template <typename Integer, typename F>
Integer findFirstFalse(Integer l, Integer r, F &&f)
{
    --l;
    ++r;
    while (r > l + 1)
    {
        Integer m = l + (r - l) / 2;
        if (f(m))
        {
            l = m;
        }
        else
        {
            r = m;
        }
    }
    return r;
}
template <typename Integer>
Integer fastBinPow(Integer a, Integer p)
{
    Integer ans = 1;
    while (p)
    {
        if (p % 2)
            ans *= a;

        p /= 2;
        a *= a;
    }
    return ans;
}

// Function to draw the image using SFML
void drawImage(sf::RenderWindow& window, FSeries& fourie, int newWidth, int newHeight)
{
    sf::Image image;
    image.create(newWidth, newHeight, sf::Color::White);

    sf::Texture texture;
    texture.create(newWidth, newHeight);
    sf::Sprite sprite(texture);

    const double stepX = 0.8 / newWidth;
    const double stepY = 0.8 / newHeight;

    sf::Vector2f point;
    for (int i = 0; i < newHeight; ++i)
    {
        point.y = 0.1 + i * stepY;

        for (int j = 0; j < newWidth; ++j)
        {
            point.x = 0.1 + j * stepX;

            double value = fourie.value_in_point({point.x, point.y});
            value = max(min(1., value), 0.);

            // Map the value to the range [0, 255] for grayscale image
            unsigned char pixelValue = static_cast<unsigned char>(value * 255.0);

            // Set pixel color
            image.setPixel(j, i, sf::Color(pixelValue, pixelValue, pixelValue));
        }
    }

    texture.update(image);
    window.clear();
    window.draw(sprite);
    window.display();
}


double integral(int data_size, function<double(double)> f)
{
    double M = f(0);
    double m = M;

    vector<double> values(data_size);
    values[0] = M;

    for (int i = 1; i < data_size; i++)
    {
        values[i] = f(i);

        M = max(values[i], M);
        m = min(values[i], m);
    }

    double ans = 0.;
    M -= m;

    if (abs(M) > EPS)
    {
        for (int i = 0; i < data_size; i++)
        {
            ans += (values[i] - m) / M;
        }
    }
    else
    {
        ans = 1.;
    }

    double V = (R - L) * M;

    ans = ans * V / data_size;
    ans += m * (R - L);

    return ans;
}



int main(int argc, char *argv[])
{
    string output_path = "approximation.txt";
    if(argc == 2)
    {
        output_path = argv[1];
    }
    cout << "Output path: " << output_path << "\n";

    int dimensions, data_size, fourie_size;

    int originalWidth, originalHeight;
    cin >> originalWidth >> originalHeight;

    dimensions = 2;
    data_size = originalHeight * originalWidth;
    fourie_size = data_size/30;

    vector<vector<double>> points(data_size, vector<double>(dimensions));
    vector<double> values(data_size);

    for (int i = 0; i < data_size; i++)
    {
        for (auto &j : points[i])
        {
            cin >> j;
        }
        cin >> values[i];
    }

    const int newWidth  = 4 * originalWidth;  // Ширина изображения с увеличенным разрешением
    const int newHeight = 4 * originalHeight; // Высота изображения с увеличенным разрешением
    sf::RenderWindow window(sf::VideoMode(newWidth, newHeight), output_path);

    // Перебираем все sin(m1 * x1 + m2 * x2 + ... + mn * xn) и cos(...)
    // mi = {0, 1, ... , M-1}
    auto comp = [&](int i)
    {
        return fastBinPow(ll(i), ll(dimensions)) < 2 * fourie_size;
    };
    int M = (findFirstFalse(0, fourie_size, comp)+1)/2;

    int cnt = 1;
    vector<int> m(dimensions, 0);

    FSeries fourie;

    m[0]++;

    auto constant_f = [&](int i)
    {
        return values[i];
    };

    fourie.C = integral(data_size, constant_f);

    while (cnt < fourie_size)
    {
        auto cos_integral = [&](int i)
        {
            double theta = 0;
            for (int j = 0; j < dimensions; j++)
            {
                theta += m[j] * points[i][j];
            }
            return cos(2 * M_PI * theta) * values[i];
        };

        auto sin_integral = [&](int i)
        {
            double theta = 0;
            for (int j = 0; j < dimensions; j++)
            {
                theta += m[j] * points[i][j];
            }
            return sin(2 * M_PI * theta) * values[i];
        };

        double sin_k = 2 * integral(data_size, sin_integral);
        double cos_k = 2 * integral(data_size, cos_integral);


        if (abs(sin_k) > EPS_K)
        {
            auto sin_term = [=](const vector<double> &point)
            {
                double theta = 0;
                for(int j=0; j<dimensions; j++)
                {
                    theta += m[j]*point[j];
                }
                return sin(2 * M_PI * theta);
            };

            fourie.terms.push_back(sin_term);
            fourie.k.push_back(sin_k);
        }

        if (abs(cos_k) > EPS_K)
        {
            auto cos_term = [=](const vector<double> &point)
            {
                double theta = 0;
                for(int j=0; j<dimensions; j++)
                {
                    theta += m[j]*point[j];
                }
                return cos(2 * M_PI * theta);
            };

            fourie.terms.push_back(cos_term);
            fourie.k.push_back(cos_k);
        }

        cnt++;
        cout<<output_path<<" | Approximation | progress : "<< cnt << "/" << fourie_size <<"\n";
        if(cnt%400 == 0)
        {
            drawImage(window, fourie, newWidth, newHeight);
        }

        int i = 0;
        m[i]++;

        while (m[i] > M)
        {
            m[i] = -M + 1;
            m[++i]++;
            if (i == dimensions)
                break;
        }
        if (i == dimensions)
            break;
    }

    // Дальше просто генерируем много случайных точек
    // Можно вынести в функцию

    ofstream output(output_path);
    if (!output.is_open())
    {
        cout << "Не удалось открыть файл." << endl;
        return 0;
    }

    const int NUMBER_POINTS = newWidth * newHeight;
    output << newWidth << " " << newHeight << "\n";

    vector<double> new_point(dimensions);

    sf::Image image;
    image.create(newWidth, newHeight, sf::Color::White);

    sf::Texture texture;
    texture.create(newWidth, newHeight);
    sf::Sprite sprite(texture);

    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            double normalized_j = (static_cast<double>(j) / (newWidth));
            double normalized_i = (static_cast<double>(i) / (newHeight));

            new_point[0] = normalized_j;
            new_point[1] = normalized_i;

            double value = fourie.value_in_point(new_point);

            output << new_point[0] << " " << new_point[1] << " ";
            output << value << "\n";

            value = max(min(1., value), 0.);

            // Map the value to the range [0, 255] for grayscale image
            unsigned char pixelValue = static_cast<unsigned char>(value * 255.0);

            // Set pixel color
            image.setPixel(j, i, sf::Color(pixelValue, pixelValue, pixelValue));
        }

        texture.update(image);
        window.clear();
        window.draw(sprite);
        window.display();
    }

    output.close();
    cout << "Файл успешно записан." << endl;
    cout << fourie.terms.size() << endl;
    return 0;
}