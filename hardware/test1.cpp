#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#include <omp.h>

int main()
{
    long n = 10000;
    // vector<double> in(n * n);
    // vector<double> out(n * n);
    double *in = (double *)malloc(sizeof(double) * n * n);
    double *out = (double *)malloc(sizeof(double) * n * n);

    #pragma omp parallel for
    for (long j = 0; j < n; j++)
    {
        for (long i = 0; i < n; i++)
        {
            in[i + j * n] = 1.0 / (1.0 + i);
            out[i + j * n] = 1.0 / (1.0 + 2.0 * i);
        }
    }

    double start = omp_get_wtime();
    long repeats = 25;
    for (long r = 0; r < repeats; r++)
    {

        #pragma omp parallel for
        for (long j = 0; j < n; j++)
        {
            for (long i = 0; i < n; i++)
            {
                out[i + j * n] = in[i + j * n];
            }
        }
        swap(in, out);
        // in.swap(out);
    }
    double stop = omp_get_wtime();
    double time = (stop - start) / repeats;
    // cout << "Exec time: " << time << " s" << endl;
    cout << "MB: " << (2 * n * n * sizeof(double)) / time / 1e9 << " GB/s" << endl;
}
