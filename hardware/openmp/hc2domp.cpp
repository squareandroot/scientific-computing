#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>
#include <omp.h>

using namespace std;

void solve(long x_steps, long y_steps, long t_steps, ofstream &fs)
{
    double delta_x = 1.0 / (x_steps - 1.0);
    double delta_y = 1.0 / (y_steps - 1.0);
    double delta_t = 1.0 / (t_steps - 1.0);

    vector<double> prev_temp(x_steps * y_steps);
    vector<double> new_temp(x_steps * y_steps);

    #pragma omp parallel for
    for (int j = 0; j < y_steps; j++)
    {
        for (int i = 0; i < x_steps; i++)
        {
            double x = i * delta_x;
            double y = j * delta_y;
            // prev_temp[i + x_steps * j] = -250 * (pow(x - 0.5, 2) + pow(y - 0.5, 2)) + 350;
            prev_temp[i + x_steps * j] = pow(x, 2) + pow(y, 2);
            new_temp[i + x_steps * j] = prev_temp[i + x_steps * j];
        }
    }

    double alpha_x = delta_t / pow(delta_x, 2);
    double alpha_y = delta_t / pow(delta_y, 2);

    double start = omp_get_wtime();

    for (int k = 1; k < t_steps; k++)
    {

        #pragma omp parallel for
        for (int j = 1; j < y_steps - 1; j++)
        {
            for (int i = 1; i < x_steps - 1; i++)
            {
                new_temp[i + x_steps * j] =
                    prev_temp[i + x_steps * j] + alpha_x * (prev_temp[i - 1 + x_steps * j] - 2 * prev_temp[i + x_steps * j] + prev_temp[i + 1 + x_steps * j]) + alpha_y * (prev_temp[i + x_steps * (j - 1)] - 2 * prev_temp[i + x_steps * j] + prev_temp[i + x_steps * (j + 1)]);
            }
        }
        prev_temp.swap(new_temp);
    }

    double stop = omp_get_wtime();

    double time = (stop - start);
    cout << "Memory Bandwidth @ " << x_steps << " steps : "
         << (2 * x_steps * y_steps * (t_steps - 1) * sizeof(double)) / time / 1e9 << " GB/s"
         << " took " << time << " s" << endl;

    fs << x_steps << " " << (2 * x_steps * y_steps * (t_steps - 1) * sizeof(double)) / time / 1e9 << " " << time << endl;
}

int main()
{
    int n = 13;
    long xy_steps[n - 3];
    long t_steps[n - 3];

    for (int i = 0; i < n - 3; i++)
    {
        xy_steps[i] = pow(2, i + 4);
        t_steps[i] = 20000;
    }

    ofstream fs("out.txt");

    for (int i = 0; i < n - 3; i++)
    {
        solve(xy_steps[i], xy_steps[i], t_steps[i], fs);
    }

    fs.close();
}
