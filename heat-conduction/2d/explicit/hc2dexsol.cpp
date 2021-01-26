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

    int x_steps = 100;
    int y_steps = 100;
    int t_steps = 40000;
    double delta_x = 1.0 / (x_steps - 1.0);
    double delta_y = 1.0 / (y_steps - 1.0);
    double delta_t = 1.0 / (t_steps - 1.0);

    int output_n = 1000;
    const string path = "/home/andreas/scientific-computing/heat-conduction/2d/explicit/out/";

    vector<double> prev_temp(x_steps * y_steps);
    vector<double> new_temp(x_steps * y_steps);

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

    stringstream ss;
    ss << path << "out-t0.dat";

    ofstream fs(ss.str().c_str());

    for (int j = 0; j < y_steps; j++)
    {
        for (int i = 0; i < x_steps; i++)
        {
            fs << i * delta_x << " " << j * delta_y << " " << prev_temp[i + x_steps * j] << endl;
        }
    }
    fs.close();

    double alpha_x = delta_t / pow(delta_x, 2);
    double alpha_y = delta_t / pow(delta_y, 2);

    for (int k = 1; k < t_steps; k++)
    {
        for (int j = 1; j < y_steps - 1; j++)
        {
            for (int i = 1; i < x_steps - 1; i++)
            {
                new_temp[i + x_steps * j] =
                    prev_temp[i + x_steps * j] + alpha_x * (prev_temp[i - 1 + x_steps * j] - 2 * prev_temp[i + x_steps * j] + prev_temp[i + 1 + x_steps * j]) + alpha_y * (prev_temp[i + x_steps * (j - 1)] - 2 * prev_temp[i + x_steps * j] + prev_temp[i + x_steps * (j + 1)]);
            }
        }

        if (k % output_n == 0 || k == t_steps - 1)
        {
            stringstream ss;
            ss << path << "out-t" << k << ".dat";

            ofstream fs(ss.str().c_str());

            for (int j = 0; j < y_steps; j++)
            {
                for (int i = 0; i < x_steps; i++)
                {
                    fs << i * delta_x << " " << j * delta_y << " " << prev_temp[i + x_steps * j] << endl;
                    prev_temp[i + x_steps * j] = new_temp[i + x_steps * j];
                }
            }
            fs.close();
        }
        else
        {
            for (int j = 0; j < y_steps; j++)
            {
                for (int i = 0; i < x_steps; i++)
                {
                    prev_temp[i + x_steps * j] = new_temp[i + x_steps * j];
                }
            }
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;
}
