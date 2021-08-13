#ifndef INCLUDE_HPP_CORESPEEDOMETER_
#define INCLUDE_HPP_CORESPEEDOMETER_


#include <Windows.h>


class CoreSpeedometer
{
public:
    CoreSpeedometer();

public:
    double GetCoreSpeed();

private:
    void Sample();

private:
    double coreSpeed_;
    DWORD prevClockPoint_;
    ULARGE_INTEGER prevTscPoint_;
};


#endif // INCLUDE_HPP_CORESPEEDOMETER_
