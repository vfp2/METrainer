#ifndef INCLUDE_HPP_STATSKEEPER_
#define INCLUDE_HPP_STATSKEEPER_


#include "psistats.h"


class StatsKeeper
{
public:
    StatsKeeper();

public:
    void Push(char inBit);
    void Reset();

    double Total();
    double HitRate();
    double ZScore();
    double PValue();
    static double Betai(double ones, double total);

private:
    double total_;
    double ones_;
    CPsiStats psiStats_;
};


#endif // INCLUDE_HPP_STATSKEEPER_
