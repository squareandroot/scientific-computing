#include <iostream>
#include <lapacke.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>

using namespace std;

// double initialValue(const double &x, const double &y)
// {
//     return -250 * (pow(x - 0.5, 2) + pow(y - 0.5, 2)) + 350;
// }

double initialValue(const double &x, const double &y)
{
    return pow(x, 2) + pow(y, 2);
}

int main()
{
    auto t1 = chrono::high_resolution_clock::now();

    int x_steps = 200;
    int y_steps = 200;
    int t_steps = 200;
    double delta_x = 1.0 / (x_steps - 1.0);
    double delta_y = 1.0 / (y_steps - 1.0);
    double delta_t = 1.0 / (t_steps - 1.0);
    double alpha_x = delta_t / pow(delta_x, 2);
    double alpha_y = delta_t / pow(delta_y, 2);

    int n_x = x_steps - 2;
    int n_y = y_steps - 2;

    int output_n = 5;
    const string path = "/home/andreas/scientific-computing/heat-conduction/2d/implicit/out/";

    vector<double> A_x(4 * n_x);

    for (int i = 0; i < n_x; i++)
    {
        A_x[1 + 4 * i] = -alpha_x;
        A_x[2 + 4 * i] = 1.0 + 2.0 * alpha_x;
        A_x[3 + 4 * i] = -alpha_x;
    }

    vector<double> A_y(4 * n_y);

    for (int i = 0; i < n_y; i++)
    {
        A_y[1 + 4 * i] = -alpha_y;
        A_y[2 + 4 * i] = 1.0 + 2.0 * alpha_y;
        A_y[3 + 4 * i] = -alpha_y;
    }

    vector<lapack_int> P_x(n_x);
    vector<lapack_int> P_y(n_y);

    vector<double> U0(n_x * n_y);

    for (int j = 0; j < n_y; j++)
    {
        for (int i = 0; i < n_x; i++)
        {
            double x = (i + 1) * delta_x;
            double y = (j + 1) * delta_y;
            U0[i + n_x * j] = initialValue(x, y);
        }
    }

    vector<double> out_x(n_y);
    vector<double> out_y(n_x);

    stringstream ss;
    ss << path << "out-t0.dat";

    ofstream fs(ss.str().c_str());

    for (int i = 0; i < x_steps; i++)
    {
        fs << i * delta_x << " " << 0.0 << " " << initialValue(i * delta_x, 0) << endl;
    }

    for (int j = 0; j < n_y; j++)
    {
        fs << 0.0 << " " << delta_y * (j + 1) << " " << initialValue(0, delta_y * j) << endl;

        for (int i = 0; i < n_x; i++)
        {
            fs << (i + 1) * delta_x << " " << (j + 1) * delta_y << " " << U0[i + n_x * j] << endl;
        }

        fs << 1.0 << " " << delta_y * (j + 1) << " " << initialValue(1, delta_y * j) << endl;
    }

    for (int i = 0; i < x_steps; i++)
    {
        fs << i * delta_x << " " << 1.0 << " " << initialValue(i * delta_x, 1) << endl;
    }

    fs.close();

    for (int k = 1; k < t_steps; k++)
    {
        for (int j = 0; j < n_y; j++)
        {
            // out_y = vector<double>(U0.begin() + n_x * j, U0.begin() + n_x * (j + 1));
            for (int i = 0; i < n_x; i++)
            {
                out_y[i] = U0[i + n_x * j];
            }

            out_y[0] += alpha_x * initialValue(0, delta_y * (j + 1));
            out_y[n_x - 1] += alpha_x * initialValue(1, delta_y * (j + 1));

            vector<double> A_temp = A_x;

            int info = LAPACKE_dgbsv(LAPACK_COL_MAJOR, n_x, 1, 1, 1, &A_temp[0], 4, &P_x[0], &out_y[0], n_x);
            if (info != 0)
            {
                cout << "Error in dgbsv " << info << endl;
                exit(1);
            }

            for (int i = 0; i < n_x; i++)
            {
                U0[i + n_x * j] = out_y[i];
            }
        }

        for (int i = 0; i < n_x; i++)
        {
            for (int j = 0; j < n_y; j++)
            {
                out_x[j] = U0[i + n_x * j];
            }

            out_x[0] += alpha_y * initialValue(delta_x * (i + 1), 0);
            out_x[n_y - 1] += alpha_y * initialValue(delta_x * (i + 1), 1);

            vector<double> A_temp = A_y;

            int info = LAPACKE_dgbsv(LAPACK_COL_MAJOR, n_y, 1, 1, 1, &A_temp[0], 4, &P_y[0], &out_x[0], n_y);
            if (info != 0)
            {
                cout << "Error in dgbsv " << info << endl;
                exit(1);
            }

            for (int j = 0; j < n_y; j++)
            {
                U0[i + n_x * j] = out_x[j];
            }
        }

        if (k % output_n == 0 || k == t_steps - 1)
        {
            stringstream ss;
            ss << path << "out-t" << k << ".dat";

            ofstream fs(ss.str().c_str());

            for (int i = 0; i < x_steps; i++)
            {
                fs << i * delta_x << " " << 0.0 << " " << initialValue(i * delta_x, 0) << endl;
            }

            for (int j = 0; j < n_y; j++)
            {
                fs << 0.0 << " " << delta_y * (j + 1) << " " << initialValue(0, delta_y * j) << endl;

                for (int i = 0; i < n_x; i++)
                {
                    fs << (i + 1) * delta_x << " " << (j + 1) * delta_y << " " << U0[i + n_x * j] << endl;
                }

                fs << 1.0 << " " << delta_y * (j + 1) << " " << initialValue(1, delta_y * j) << endl;
            }

            for (int i = 0; i < x_steps; i++)
            {
                fs << i * delta_x << " " << 1.0 << " " << initialValue(i * delta_x, 1) << endl;
            }
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;
}
