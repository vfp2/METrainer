#include "StatsGroup.hpp"
#include <assert.h>


StatsGroup::StatsGroup(int trialWindowLength)
    : trialWindow_(new int[trialWindowLength])
    , trialWindowLength_(trialWindowLength)
{
    assert(trialWindowLength > 0);

    Reset();
}


StatsGroup::~StatsGroup()
{
    delete trialWindow_;
}


void StatsGroup::Feed(int val)
{
    trialWindowSum_ -= trialWindow_[trialWindowPos_];
    trialWindow_[trialWindowPos_] = val;
    trialWindowSum_ += val;

    ++trialWindowPos_ %= trialWindowLength_;

    trialStats_.Push((char)val);
}


void StatsGroup::Reset()
{
    int val = 1;

    trialWindowPos_ = 0;
    trialWindowSum_ = 0;
    for (int i=0; i<trialWindowLength_; ++i)
    {
        trialWindow_[i] = val;
        trialWindowSum_ += val;
        val = 1 - val;
    }
    trialWindow_[0] = 0;
    trialWindow_[1] = 1;

    trialStats_.Reset();
}


double StatsGroup::GetWindowHitRate()
{
    return (double)trialWindowSum_/(double)trialWindowLength_;
}


double StatsGroup::GetWindowPValue()
{
    return StatsKeeper::Betai(trialWindowSum_, trialWindowLength_);
}


double StatsGroup::GetTrialTotal()
{
    return trialStats_.Total();
}


double StatsGroup::GetTrialHitRate()
{
    return trialStats_.HitRate();
}


double StatsGroup::GetTrialPValue()
{
    return trialStats_.PValue();
}
