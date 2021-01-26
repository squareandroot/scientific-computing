#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    auto t1 = chrono::high_resolution_clock::now();

    int x_steps = 10;
    int t_steps = 200;
    double delta_x = 1.0 / (x_steps - 1.0);
    double delta_t = 1.0 / (t_steps - 1.0);

    int output_n = 201;
    const string path = "/home/andreas/scientific-computing/heat-conduction/explicit/out/";

    vector<double> prev_temp(x_steps);
    vector<double> new_temp(x_steps);

    for (int i = 0; i < x_steps; i++)
    {
        double x = i * delta_x;
        prev_temp[i] = 350 - 100 * x + 200 * sin(M_PI * x);
        new_temp[i] = prev_temp[i];
    }

    stringstream ss;
    ss << path << "out-explicit-t1.dat";

    ofstream fs(ss.str().c_str());
    for (int j = 0; j < x_steps; j++)
    {
        fs << j * delta_x << " " << new_temp[j] << endl;
    }
    fs.close();

    double alpha = delta_t / pow(delta_x, 2);

    for (int i = 1; i < t_steps; i++)
    {
        for (int j = 1; j < x_steps - 1; j++)
        {
            new_temp[j] = prev_temp[j] + alpha * (prev_temp[j - 1] - 2 * prev_temp[j] + prev_temp[j + 1]);
        }

        if (i % output_n == 0 || i == t_steps - 1)
        {
            stringstream ss;
            ss << path << "out-explicit-t" << i + 1 << ".dat";

            ofstream fs(ss.str().c_str());

            for (int j = 0; j < x_steps; j++)
            {
                fs << j * delta_x << " " << new_temp[j] << endl;
            }
            fs.close();
        }

        for (int k = 1; k < x_steps - 1; k++)
        {
            prev_temp[k] = new_temp[k];
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " us" << endl;
}
