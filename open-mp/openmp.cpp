#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <omp.h>

using namespace std;

struct Point
{
    Point(double x, double y) : x{x}, y{y} {}
    double x;
    double y;
};

class Polygon
{
public:
    int n = 987654321;
    Polygon() {}

    Polygon(int countPoint)
    {
        n = countPoint;
    }

    Point getPoint(int i)
    {
        double x = 300 + i * cos(2 * M_PI * i / n);
        double y = 300 + i * sin(2 * M_PI * i / n);
        return Point{x, y};
    }

    double areaOpenMP(int const N)
    {
        double start = omp_get_wtime();
        double t = 0;
#pragma omp parallel for reduction(+ : t) num_threads(N)
        for (int i = 0; i < n; ++i)
        {
            Point point_1 = getPoint(i);
            Point point_2 = getPoint((i + 1) % n);
            t += point_1.x * point_2.y;
            t -= point_1.y * point_2.x;
        }
        if (t < 0)
            t = -t;
        std::cout << t / 2 << std::endl;
        return (float)(omp_get_wtime() - start);
    }
};

int main(int argc, char **argv)
{
    Polygon pol = Polygon();
    for (int i = 1; i <= 8; ++i)
    {
        std::cout << "Execution time using OpenMP and " << i << " threads for area = "
                  << pol.areaOpenMP(i) << " seconds"
                  << std::endl;
    }
    return 0;
}