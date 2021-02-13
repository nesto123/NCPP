#include "calculator.h"
#include <cmath>
#include <list>
#include <QList>

#define _USE_MATH_DEFINES // za pi od c++20

Calculator::Calculator(QObject *parent) : QObject(parent)
{

}

QList<double> Calculator::calc(double T, double P, double a, double b)
{
    double R = 8.314462; //  Plinska konstanta

    double A = P;
    double B = R * T * (-1);
    double C = (a / std::sqrt(T)) - b * (b * P + R * T); //-1 * std::pow(b,2) * P - R * T * b + a / std::sqrt(T);
    double D = -1 * a * b / std::sqrt(T);

    double p = (3 * A * C - std::pow(B, 2)) / (3 * std::pow(A, 2));
    double q = ((2 * std::pow(B, 3)) - (9 * A * B * C) + (27 * std::pow(A, 2) * D)) / (27 * std::pow(A, 3));

    double delta = std::pow(q / 2, 2) + std::pow(p / 3, 3);
    double rj[4];

    if (std::abs(delta) == delta)
    { // delta >= 0
        if (delta == 0.0)
        {
            rj[0] = 2 * std::cbrt(-1 * q / 2) - B / 3 / A;
            rj[1] = std::cbrt(q / 2) - B / 3 / A;
            rj[2] = std::cbrt(q / 2) - B / 3 / A;
            rj[3] = 3;
        }
        else
        { // delta > 0
            double korjen = std::sqrt(std::pow(q, 2) / 4 + std::pow(p, 3) / 27);
            rj[0] = std::cbrt((-1) * q / 2 + korjen) + std::cbrt((-1) * q / 2 - korjen) - B / (3 * A);
            rj[3] = 1;
        }
    }
    else
    { // delta < 0
        double prvi = 2 * std::cbrt(std::sqrt(-1 * std::pow(p, 3))) / std::sqrt(3);
        double zadnji = B / 3 / A;
        double r = std::sqrt(pow(-1 * p / 3, 3));
        double phi = std::acos(-1 * q / (2 * r));
        for (size_t k = 0; k < 3; k++)
            rj[k] = prvi * std::cos((phi + (2.0 * M_PI * ((double)k))) / 3.0) - zadnji;
        rj[3] = 3;
    }
    std::list<double> ints(std::begin(rj), std::end(rj));
    return QList<double>(ints.begin(), ints.end());
}

