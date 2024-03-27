#include <stdio.h>
#include <numeric>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <math.h>

using namespace std;

double vectorNorm(const double *first, const double *second, int len)
{
    return sqrt(inner_product(first, first + len, second, 0.0L));
}

double vectorNormalize(double *first, int len, double *second)
{
    double norm = vectorNorm(first, first, len);
    transform(first, first + len, second, [norm](double &c)
              { return c / norm; });
    return norm;
}

void cross(const double *first, const double *second, double *out)
{
    out[0] = first[1] * second[2] - first[2] * second[1];
    out[1] = first[2] * second[0] - first[0] * second[2];
    out[2] = first[0] * second[1] - first[1] * second[0];
}

void rotm2quat(const double *rotx, const double *roty, const double *rotz, double *quat)
{
    double R[3][3] = {{rotx[0], roty[0], rotz[0]},
                      {rotx[1], roty[1], rotz[1]},
                      {rotx[2], roty[2], rotz[2]}};
    double trace = R[0][0] + R[1][1] + R[2][2];
    double S;
    if (trace > 0)
    {
        S = sqrt(trace + 1.0) * 2;
        quat[3] = 0.25 * S;
        quat[0] = (R[2][1] - R[1][2]) / S;
        quat[1] = (R[0][2] - R[2][0]) / S;
        quat[2] = (R[1][0] - R[0][1]) / S;
    }
    else if ((R[0][0] > R[1][1]) && (R[0][0] > R[2][2]))
    {
        S = sqrt(1.0 + R[0][0] - R[1][1] - R[2][2]) * 2;
        quat[3] = (R[2][1] - R[1][2]) / S;
        quat[0] = 0.25 * S;
        quat[1] = (R[0][1] + R[1][0]) / S;
        quat[2] = (R[0][2] + R[2][0]) / S;
    }
    else if (R[1][1] > R[2][2])
    {
        S = sqrt(1.0 + R[1][1] - R[0][0] - R[2][2]) * 2;
        quat[3] = (R[0][2] - R[2][0]) / S;
        quat[0] = (R[0][1] + R[1][0]) / S;
        quat[1] = 0.25 * S;
        quat[2] = (R[1][2] + R[2][1]) / S;
    }
    else
    {
        S = sqrt(1.0 + R[2][2] - R[0][0] - R[1][1]) * 2;
        quat[3] = (R[1][0] - R[0][1]) / S;
        quat[0] = (R[0][2] + R[2][0]) / S;
        quat[1] = (R[1][2] + R[2][1]) / S;
        quat[2] = 0.25 * S;
    }
}

void acc2quat(const double *acc, const double yaw, double *quat)
{
    double thrust[] = {acc[0], acc[1], acc[2] + 9.81};
    vectorNormalize(thrust, 3, thrust);
    double x_world[] = {cos(yaw), sin(yaw), 0.};
    double y_body[3];
    double x_body[3];
    cross(thrust, x_world, y_body);
    vectorNormalize(y_body, 3, y_body);
    cross(y_body, thrust, x_body);
    vectorNormalize(x_body, 3, x_body);
    rotm2quat(x_body, y_body, thrust, quat);
}