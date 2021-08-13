#ifndef INLCUDE_HPP_QWPRD4_
#define INLCUDE_HPP_QWPRD4_


#include "IPrd.hpp"
class IRng;


class QwPrd4 : public IPrd
{
public:
    QwPrd4(IRng* rng_);
    ~QwPrd4();

public:
    virtual double Generate();

private:
    void SetTargets();

private:
    int period_;
    char* prdBuffer_;
    char* targetBuffer_;
};


#endif // INLCUDE_HPP_QWPRD4_
