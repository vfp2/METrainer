#ifndef INCLUDE_HPP_STATSGROUP_
#define INCLUDE_HPP_STATSGROUP_


#include "StatsKeeper.hpp"


class StatsGroup
{
public:
    StatsGroup(int trialWindowLength);
    ~StatsGroup();

public:
    void Feed(int val);
    void Reset();

    double GetWindowHitRate();
    double GetWindowPValue();
    double GetTrialTotal();
    double GetTrialHitRate();
    double GetTrialPValue();

private:
    int* trialWindow_;
    int trialWindowLength_;
    int trialWindowSum_;
    int trialWindowPos_;

    StatsKeeper trialStats_;
};


#endif // INCLUDE_HPP_STATSGROUP_
