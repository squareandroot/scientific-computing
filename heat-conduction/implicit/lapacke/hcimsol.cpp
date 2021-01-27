#include <iostream>
#include <lapacke.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>

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

    vector<double> A(4 * n);

    for (int i = 0; i < n; i++)
    {
        A[1 + 4 * i] = -delta_t / pow(delta_x, 2);
        A[2 + 4 * i] = 1.0 + 2.0 * delta_t / pow(delta_x, 2);
        A[3 + 4 * i] = -delta_t / pow(delta_x, 2);
    }

    vector<lapack_int> P(n);
    vector<double> u0(n, 0.0);

    for (int i = 0; i < n; i++)
    {
        double x = (i + 1) * delta_x;
        u0[i] = 350 - 100 * (x) + 200 * sin(M_PI * x);
    }

    vector<double> out(n);
    out = u0;

    stringstream ss;
    ss << path << "out-lapacke-t1.dat";

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

        vector<double> A_temp = A;

        int info = LAPACKE_dgbsv(LAPACK_COL_MAJOR, n, 1, 1, 1, &A_temp[0], 4, &P[0], &out[0], n);
        if (info != 0)
        {
            cout << "Error in dgbsv " << info << endl;
            exit(1);
        }

        if (i % output_n == 0 || i == t_steps - 1)
        {
            stringstream ss;
            ss << path << "out-lapacke-t" << i + 1 << ".dat";

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
