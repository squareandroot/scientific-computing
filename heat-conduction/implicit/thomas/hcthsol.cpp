#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;

int main()
{
    auto t1 = chrono::high_resolution_clock::now();

    int x_steps = 100;
    int t_steps = 100;
    double delta_x = 1.0 / (x_steps - 1.0);
    double delta_t = 1.0 / (t_steps - 1.0);
    double theta_0 = 350;
    double theta_1 = 250;

    int n = x_steps - 2;

    int output_n = 1;
    const string path = "./out/";

    vector<double> A(2 * n + 1);

    for (int i = 0; i < 2 * n + 1; i++)
    {
        if (i < n)
        {
            A[i] = 1.0 + 2.0 * delta_t / pow(delta_x, 2);
        }
        else
        {
            A[i] = -delta_t / pow(delta_x, 2);
        }
    }

    vector<double> u0(n, 0.0);

    for (int i = 0; i < n; i++)
    {
        double x = (i + 1) * delta_x;
        u0[i] = 350 - 100 * x + 200 * sin(M_PI * x);
    }

    vector<double> out(n);
    out = u0;

    vector<double> c(n);
    c[0] = A[0];

    stringstream ss;
    ss << path << "out-thomas-t1.dat";

    ofstream fs(ss.str().c_str());
    fs << 0 << " " << theta_0 << endl;
    for (int j = 0; j < n; j++)
    {
        fs << (j + 1) * delta_x << " " << out[j] << endl;
    }
    fs << 1 << " " << theta_1 << endl;
    fs.close();

    for (int i = 1; i < t_steps; i++)
    {
        out[0] += theta_0 * delta_t / pow(delta_x, 2);
        out[n - 1] += theta_1 * delta_t / pow(delta_x, 2);

        for (int j = 1; j < n; j++)
        {
            c[j] = A[j] - pow(A[n + j - 1], 2) / c[j - 1];
            out[j] = out[j] - A[n + j - 1] / c[j - 1] * out[j - 1];
        }

        out[n - 1] = out[n - 1] / c[n - 1];

        for (int j = n - 2; j >= 0; j--)
        {
            out[j] = (out[j] - A[j + n] * out[j + 1]) / c[j];
        }

        if (i % output_n == 0 || i == t_steps - 1)
        {
            stringstream ss;
            ss << path << "out-thomas-t" << i + 1 << ".dat";

            ofstream fs(ss.str().c_str());
            fs << 0 << " " << theta_0 << endl;
            for (int j = 0; j < n; j++)
            {
                fs << (j + 1) * delta_x << " " << out[j] << endl;
            }
            fs << 1 << " " << theta_1 << endl;
            fs.close();
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " us" << endl;
}
