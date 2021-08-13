#ifndef INCLUDE_HPP_IPRD_
#define INCLUDE_HPP_IPRD_


class IRng;


class IPrd
{
public:
    IPrd(IRng* rng);

public:
    virtual void Start();
    virtual void Stop();
    virtual void Reset();
    virtual void SetMode(int mode);
    virtual int GetMode();
    virtual double Generate();

protected:
    IRng* rng_;
    int mode_;
};


#endif // INCLUDE_HPP_IPRD_
