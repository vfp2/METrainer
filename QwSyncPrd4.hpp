#ifndef INLCUDE_HPP_QWSYNCPRD4_
#define INLCUDE_HPP_QWSYNCPRD4_


#include "IPrd.hpp"
#include "LpFilter.hpp"
#include "BackProp.H"
class IRng;

extern int netDir;
extern int netIndex;

class QwSyncPrd4 : public IPrd
{
public:
    QwSyncPrd4(IRng* rng_);
    ~QwSyncPrd4();

public:
    virtual double Generate();
    double* GetLogStreams();

private:
    void SetTargets();

private:
    LpFilter lpFilters_[7];
    double* logStreams_;
    CBackProp* annA_;
    CBackProp* annB_;
    CBackProp* annC_;
    CBackProp* annD_;
    CBackProp* annE_;
    CBackProp* annF_;
    CBackProp* annG_;
    double medianA_;
    double medianB_;
    double medianC_;
    double medianD_;
    double medianE_;
    double medianF_;
    double medianG_;
};


#endif // INLCUDE_HPP_QWSYNCPRD4_
