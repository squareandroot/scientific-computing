#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#include <omp.h>

int main()
{
    long n = 1000000;

    double start = omp_get_wtime();
    long repeats = 250;
    double s = 0.0;
    for (long r = 0; r < repeats; r++)
    {
        double h = 1.0 / double(n);
        #pragma omp parallel for simd reduction(+:s)
        for (long i = 0; i < n; i++)
        {
            s += sin(2 * M_PI * i / double(n)) * h;
        }
    }
    double stop = omp_get_wtime();
    double time = (stop - start) / repeats;
    cout << "Exec time: " << time << " s" << "    " << s << endl;
}
