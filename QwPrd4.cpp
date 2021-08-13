#include "QwPrd4.hpp"
#include "FtdiRng.hpp"
#include "RngBuffer.hpp"
#include "MersenneTwister.hpp"
#include "stdio.h"


QwPrd4::QwPrd4(IRng* rng)
    : IPrd(rng)
    , period_(62500)
    , prdBuffer_(new char[62500])
    , targetBuffer_(new char[62500])
{ }


QwPrd4::~QwPrd4()
{
    delete[] targetBuffer_;
    delete[] prdBuffer_;
}


double QwPrd4::Generate()
{
    // if PK, fill PK targets
    if (mode_ == 0)
        SetTargets();

    // fill psi bits
    char rngBitsLeft = 0;
    char rngWord7 = 0;
    int filledBits = 0;

    while (filledBits < period_)
    {
        if (rngBitsLeft == 0)
        {
            rng_->GetBuffer()->Read(&rngWord7, 1);
            rngBitsLeft = 7;
        }

        --rngBitsLeft;
        prdBuffer_[filledBits] = (rngWord7>>rngBitsLeft)&0x1;
        ++filledBits;        
    }

    // if precog, fill precog targets
    if (mode_ == 2)
        SetTargets();

    // compare psi bits to targets in random walk
/*    int walkDistance = 0;
    for (int i=0; i<period_; ++i)
    {
        if (prdBuffer_[i] == targetBuffer_[i])
            ++walkDistance;
        else
            --walkDistance;
    }*/

    while ((period_%(3*37)) != 0)
        --period_;
    if ((period_%2) != 1)
        period_ -= 3*37;

    int majVote = 0;
    for (int j=0; j<37; ++j)
    {
        // compare psi to targets and split in three streams
        int walkDistance[3];
        int walkIndex = 0;
        memset(walkDistance, 0, sizeof(walkDistance));

        for (int i=0; i<(period_/37); ++i)
        {
            if (prdBuffer_[i+j*37] == targetBuffer_[i+j*37])
                ++walkDistance[walkIndex];
            else
                --walkDistance[walkIndex];
            ++walkIndex;
            walkIndex %= 3;
        }

        // order scores
        int bestWalk = 0;
        for (int i=0; i<3; ++i)
        {
            if (abs(walkDistance[i]) > abs(bestWalk))
                bestWalk = walkDistance[i];
        }
        if (bestWalk > 0)
            majVote += 1;
        else
            majVote -= 1;
    }

    // if clairvoyance, fill targets
    if (mode_ == 1)
        SetTargets();

    // return
    return (majVote > 0)? 1 : 0;
}


void QwPrd4::SetTargets()
{
    // seed prng with entropic bits
    char seedBytes[5];
    rng_->GetBuffer()->Read((char*)&seedBytes, 5);
    unsigned long prngSeed = 0;
    for (int b=0; b<5; ++b)
    {
        prngSeed |= seedBytes[b] & 0x7F;
        prngSeed <<= 7;
    }
    init_genrand(prngSeed);

    int prngBitsUsed = 32;
    unsigned long prngWord32 = 0;
    for (int i=0; i<period_; ++i)
    {
        if (mode_ == 0)
            targetBuffer_[i] = 1;
        else
        {
            if (prngBitsUsed == 32)
            {
                prngWord32 = genrand_int32();
                prngBitsUsed = 0;
            }

            targetBuffer_[i] = (char)(prngWord32>>prngBitsUsed)&0x1;
            ++prngBitsUsed;
        }
    }
}
