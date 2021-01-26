#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>
using namespace std;

int main() {
    int m = 1000000;
    float y = 3.4;      // accurate up to 1e-7
    double z = y/2;

    cout << z << endl;

    vector<double> v(m);

    auto t1 = chrono::high_resolution_clock::now();

    for(int i=0; i<m; i++) {
        v[i] = cos(2.0*M_PI*i/double(m-1));
    }

    auto t2 = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << " ms" << endl;

    ofstream fs("out1.txt");
    for(int i=0; i<m; i++) {
        double x = 2.0*M_PI*i/double(m-1);
        fs << x << " " << v[i] << endl;
    }
    fs.close();
}
