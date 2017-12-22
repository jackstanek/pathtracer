#ifndef _HELPER_HPP
#define _HELPER_HPP

#include <cassert>
#include <cmath>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>

/* For floating point comparisons to zero */
const double EPSILON = 0.001;

inline double rand_d()
{
    /* Love that C++11 */
    std::random_device rd;
    return std::bind(std::uniform_real_distribution<>(0, 1),
                     std::default_random_engine(rd()))();
}

/* Is x approximately equal to c? */
inline double is_approx(double x, double c)
{
    return std::abs(x - c) < EPSILON;
}

inline double is_zero(double x)
{
    return is_approx(x, 0.0);
}

inline double clamp(double x, double min = 0.0, double max = 1.0)
{
    assert(min < max);
    return x > min ? (x > max ? max : x) : min;
}

/* Retrieved from
   https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c */
inline bool file_exists (const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

inline double mean(std::vector<double> data)
{
    if (data.empty()) {
        return 0;
    }

    double total = 0;
    for (double d : data) {
        total += d;
    }

    return total / data.size();
}

inline double range(std::vector<double> data)
{
    double max = -INFINITY, min = INFINITY;

    for (double d : data) {
        if (d > max) {
            max = d;
        } else if (d < min) {
            min = d;
        }
    }

    return max - min;
}

#endif
