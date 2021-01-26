#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

int main()
{
    double eps = 1.0;
    while(1.0 + 0.5*eps != 1.0) {
        eps = 0.5*eps;
    }
    std::cout << "default precision (6):              " << eps << '\n'
              << "std::fixed:                         " << std::fixed << eps << '\n'
              << "std::precision(10):                 " << std::defaultfloat << std::setprecision(10) << eps << '\n'
              << "std::precision(10) & std::fixed:    " << std::fixed        << std::setprecision(10) << eps << '\n'
              << "max precision:                      " << std::defaultfloat << std::setprecision(std::numeric_limits<long double>::digits10) << eps << '\n'
              << "max precision & std::fixed:         " << std::fixed        << std::setprecision(std::numeric_limits<long double>::digits10) << eps << '\n'
    ;
}
