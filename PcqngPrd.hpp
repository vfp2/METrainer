#ifndef INLCUDE_HPP_PCQNGPRD_
#define INLCUDE_HPP_PCQNGPRD_


#include "IPrd.hpp"
#include "LfsrCorrector.hpp"
class IRng;


class PcqngPrd : public IPrd
{
public:
    PcqngPrd(IRng* rng_);

public:
    virtual double Generate();

private:
    void SetTargets(int mode);

private:
    LfsrCorrector lfsrCorrectorA_;
    LfsrCorrector lfsrCorrectorB_;
    LfsrCorrector lfsrCorrectorT_;

    char* targetBuffer_;
};



#endif // INLCUDE_HPP_PCQNGPRD_
