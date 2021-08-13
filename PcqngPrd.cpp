#include "PcqngPrd.hpp"
#include "IRng.hpp"
#include "RngBuffer.hpp"
#include <memory.h>
#include <math.h>


PcqngPrd::PcqngPrd(IRng* rng)
    : IPrd(rng)
{ }


double PcqngPrd::Generate()
{
    // Fill psi bits
    char prdBuffer[111*15];
    int prdBufferIndex = 0;
    for (int i=0; i<111; ++i)
    {
        // Get pcqng raw dequantized bits
        char rawOctet[2];
        rng_->GetBuffer()->Read(rawOctet, 2);
    
        // Corrector and XBias processing
        for (int j=0; j<15; ++j)
        {
            char bitA = lfsrCorrectorA_.Next((rawOctet[0]<<(j%8))&0x1);
            char bitB = lfsrCorrectorB_.Next((rawOctet[1]<<(j%8))&0x1);

            if (bitA == bitB)
            {
                if (bitA == 1)
                    prdBuffer[prdBufferIndex] = 1;
                else
                    prdBuffer[prdBufferIndex] = 0;
                ++prdBufferIndex;
            }
            else
            {
                --j;
            }
        }
    }

    // Fill target bits
    char targetBuffer[111*15];
    char rawOctet;
    rng_->GetBuffer()->Read(&rawOctet, 1);
    for (int i=0; i<(111*15); ++i)
    {
        targetBuffer[i] = lfsrCorrectorT_.Next((rawOctet<<(i%8))&0x1);
    }


    // Compare psi to targets and split in three streams
    int walkDistance[3];
    int walkIndex = 0;
    memset(walkDistance, 0, sizeof(walkDistance));
    for (int i=0; i<(111*15); ++i)
    {
        if (prdBuffer[i] == targetBuffer[i])
            ++walkDistance[walkIndex];
        else
            --walkDistance[walkIndex];
        ++walkIndex;
        walkIndex %= 3;
    }

    // Order scores
    int bestWalk = 0;
    for (int i=0; i<3; ++i)
    {
        if (abs(walkDistance[i]) > abs(bestWalk))
            bestWalk = walkDistance[i];
    }

    return (bestWalk > 0)? 1 : 0;
}
