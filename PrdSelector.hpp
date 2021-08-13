#ifndef INLCUDE_HPP_PRDSELECTOR_
#define INLCUDE_HPP_PRDSELECTOR_


#include "IPrd.hpp"
class IRng;
class IBuffer;


class PrdSelector : public IPrd
{
public:
    PrdSelector(IRng* rng);
    ~PrdSelector();

public:
    virtual double Generate();
    virtual void SetMode(int mode);
    virtual int GetMode();
    int GetSource();
    double* GetLogStreams();

private:
    IBuffer* rngBuffer_;
    IPrd* prd_;
    int source_;
};


#endif // INLCUDE_HPP_PRDSELECTOR_
