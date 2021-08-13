#include "StatsKeeper.hpp"
#include "math.h"

double ZtoP(double);


StatsKeeper::StatsKeeper()
    : total_(0.)
    , ones_(0.)
{}


void StatsKeeper::Push(char inBit)
{
    ones_ += inBit&0x1;
    ++total_;
}


void StatsKeeper::Reset()
{
    total_ = 0.;
    ones_ = 0.;
}


double StatsKeeper::Total()
{
    return total_;
}


double StatsKeeper::HitRate()
{
    return (total_ != 0.)? (ones_/total_) : .5;
}


double StatsKeeper::ZScore()
{
    double numtor = 2.*ones_ - total_;
    double denom  = (total_ != 0)? sqrt(total_) : 1.0;
    return (numtor/denom);
}


double StatsKeeper::PValue()
{
    if (total_ == 0)
        return .5;
    return psiStats_.betai(ones_, total_-ones_+1, .5);
}

double StatsKeeper::Betai(double ones, double total)
{
    return CPsiStats::betai(ones, total-ones+1, .5);
}
