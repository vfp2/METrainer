#include "IRng.hpp"
#include "IBuffer.hpp"


IRng::IRng(IBuffer* buffer)
    : buffer_(buffer)
    , mode_(0)
{ }


void IRng::Start()
{ }


void IRng::Stop()
{ }


IBuffer* IRng::GetBuffer()
{
    return buffer_;
}


void IRng::SetMode(int mode)
{
    mode_ = mode;
}


int IRng::GetMode()
{
    return mode_;
}
