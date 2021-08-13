#include "PcqngRng.hpp"
#include "LpFilter.hpp"
#include "IBuffer.hpp"
#include <process.h>
#include "stdio.h"


PcqngRng::PcqngRng(IBuffer* buffer)
    : IRng(buffer)
{}


void PcqngRng::Start()
{
    mmTimerEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (mmTimerEvent_ == NULL)
        throw "CreateEvent FAILED";

    // Ensure 1ms timer resolution available on this machine
    TIMECAPS timeCaps;
    timeGetDevCaps(&timeCaps, sizeof(timeCaps));
    if (timeCaps.wPeriodMin != 1)
        throw "1ms resolution not available";

    // Start the PCQNG low-speed timer interrupt
    mmTimerId_ = timeSetEvent(1, 0, (LPTIMECALLBACK)mmTimerEvent_, 0, TIME_PERIODIC | TIME_CALLBACK_EVENT_PULSE);
    if (mmTimerId_ == 0)
        throw "Could not start PCQNG";

    // Start the runner thread
    runnerThread_ = (HANDLE)_beginthreadex(NULL, 0, PcqngRng::Runner, this, 0, NULL);
}


void PcqngRng::Stop()
{
    timeKillEvent(mmTimerId_);
    CloseHandle(mmTimerEvent_);

    WaitForSingleObject(runnerThread_, INFINITE);
    CloseHandle(runnerThread_);
}


unsigned __stdcall PcqngRng::Runner(void* instance)
{
    if (SetThreadAffinityMask(GetCurrentThread(), 1) == 0)
        throw "SetThreadAffinityMask FAILED";
    Sleep(1);

//    if (SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS) == 0)
//        throw "SetPriorityClass FAILED";

    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0)
        throw "SetThreadPriority FAILED";


    PcqngRng* inst = (PcqngRng*)instance;
    LpFilter lpFilter;

    int filterState = 0;
    unsigned __int64 tscWord = 0;
    unsigned __int64 prevTscWord = 0;

    unsigned counter = 1;
    while (1)
    {
        DWORD waitResponse = WaitForSingleObject(inst->mmTimerEvent_, INFINITE);

        if (waitResponse == WAIT_OBJECT_0)
        {
            // TSC Measurement
            __asm
            {
                rdtsc
                mov dword ptr[tscWord], eax
                mov dword ptr[tscWord]+4, edx
            }

            // TSC difference
            __int64 tscDiff = (tscWord-prevTscWord);
            prevTscWord = tscWord;
    
            switch (filterState)
            {
            case 20: // Fully initialized
                {
                    double filterRatio = (double)tscDiff/lpFilter.GetValue();
                    if ((filterRatio > 1.25) || (filterRatio < 0.8))
                        lpFilter.Feed((double)tscDiff, 200);
                    else
                        lpFilter.Feed((double)tscDiff, 10);
                
                    double qFactor = lpFilter.GetValue()/33333; // Quantization
                    char eBits = (char)(((double)tscDiff / qFactor) + 0.5); // Entropic bits
                    inst->buffer_->Write(&eBits, 1);
    if ((++counter % 8192) == 0)
    {
        printf("%f  %f\n", qFactor, lpFilter.GetValue()); 
    }
                }
                continue;
            case 0: // First sample
                ++filterState;
                continue;
            case 1: // Filter init
                ++filterState;
                lpFilter.Init((double)tscDiff);
            default: // Early samples
                ++filterState;
                lpFilter.Feed((double)tscDiff, 10);
                continue;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}
