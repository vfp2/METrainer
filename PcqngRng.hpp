#ifndef INCLUDE_HPP_PCQNGRNG_
#define INCLUDE_HPP_PCQNGRNG_


#include "IRng.hpp"
#include <Windows.h>
class IBuffer;


class PcqngRng : public IRng
{
public:
    PcqngRng(IBuffer* buffer);

public:
    virtual void Start();
    virtual void Stop();

private:
    static unsigned __stdcall Runner(void* instance);
    HANDLE mmTimerEvent_;
    MMRESULT mmTimerId_;
    HANDLE runnerThread_;
};


#endif // INCLUDE_HPP_PCQNGRNG_
