#include "IPrd.hpp"
#include "IRng.hpp"
#include "IBuffer.hpp"


IPrd::IPrd(IRng* rng)
    : rng_(rng)
    , mode_(0)
{ }


void IPrd::Start()
{
    rng_->Start();
}


void IPrd::Stop()
{
    rng_->Stop();
}


void IPrd::Reset()
{
    rng_->GetBuffer()->Clear();
}


double IPrd::Generate()
{
    return -1;
}


void IPrd::SetMode(int mode)
{
    mode_ = mode;
}


int IPrd::GetMode()
{
    return mode_;
}
