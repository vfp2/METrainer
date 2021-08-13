#ifndef INLCUDE_HPP_PCQNGANNPRD_
#define INLCUDE_HPP_PCQNGANNPRD_


#include "IPrd.hpp"
#include "LfsrCorrector.hpp"
#include "LpFilter.hpp"
class IRng;


class PcqngAnnPrd : public IPrd
{
public:
    PcqngAnnPrd(IRng* rng_);

public:
    virtual double Generate();
    double* GetLogStreams();

private:
    void SetTargets();
    void FillPrdBuffer();
    double ZToP(double zScore);
    void CompareTargetsFilter(double* prdStreams);
    void ConvertZsToPsFilter(double* prdStreams);
    void SNormFilter(double* prdStreams);
    void FactorizeFilter(double* prdStreams);
    static int __cdecl PsComparator(const void* p1, const void* p2);

private:
    LfsrCorrector rngLfsrCorrector_;
    LfsrCorrector targetsLfsrCorrector_;
    LpFilter compLpFilter_;
    const int annStreamCount_;
    const double compLpFilterConst_;

    const int bufferSize_;
    char* prdBuffer_;
    char* targetBuffer_;
    double* logStreams_;
};


#endif // INLCUDE_HPP_PCQNGANNPRD_
