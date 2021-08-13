#include "PcqngAnnPrd.hpp"
#include "IRng.hpp"
#include "IBuffer.hpp"
#include "MersenneTwister.hpp"

#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


//FILE* pFile;

PcqngAnnPrd::PcqngAnnPrd(IRng* rng)
    : IPrd(rng)
    , compLpFilterConst_(25.5033)
    , annStreamCount_(7)
    , bufferSize_(222*annStreamCount_)
    , prdBuffer_(new char[bufferSize_])
    , targetBuffer_(new char[bufferSize_])
    , logStreams_(new double[annStreamCount_])
{
//    pFile = fopen("EigenNormed.txt", "w+");
    compLpFilter_.Init(0);
}


double PcqngAnnPrd::Generate()
{
    // Fill target bits psychokinesis
    if (mode_ == 0)
        SetTargets();

    // Fill psi bits
    FillPrdBuffer();

    // Fill target bits precognition
    if (mode_ == 2)
        SetTargets();

    // Compare psi bits to targets and apply low pass filter and normalize
    double* prdStreams = new double[annStreamCount_];
    CompareTargetsFilter(prdStreams);

    // Save for logging
    memcpy(logStreams_, prdStreams, 7*sizeof(double));

    // Comvert from z-scores to p-values
    ConvertZsToPsFilter(prdStreams);

    // Sort ascending
    qsort((void*)prdStreams, (size_t)annStreamCount_, sizeof(double), PcqngAnnPrd::PsComparator);

    // Normalize to SNorm
    SNormFilter(prdStreams);

    // Factorize
    FactorizeFilter(prdStreams);

    for (int i=0; i<annStreamCount_; ++i)
    {
        printf("%f  ", prdStreams[i]);
//        fprintf(pFile, "%f  ", prdStreams[i]);
    }
//    fprintf(pFile, "\n");
    printf("\n");

    double retVal = prdStreams[0];
    delete[] prdStreams;

    // Fill target bits clairvoyance
    if (mode_ == 1)
        SetTargets();

    return retVal;
}


void PcqngAnnPrd::SetTargets()
{
    char rawOctets[5];
    rng_->GetBuffer()->Read(rawOctets, 5);

    // Seed the twister
    int i = 0;
    unsigned long prngSeed = 0;
    for (i=0; i<32; ++i)
    {
        prngSeed <<= 1;
        prngSeed |= targetsLfsrCorrector_.Next((rawOctets[i/annStreamCount_]>>(i%7))&0x1);
    }
    init_genrand(prngSeed);

    int prngBitsUsed = 32;
    unsigned long prngWord = 0;
    switch (mode_)
    {
    case 0:  // Psychokinesis (0)
        for (i=0; i<bufferSize_; ++i)
        {
            targetBuffer_[i] = 1;
        }
        break;
    default:  // Clairvoyance (1) or Precognition (2)
        for (i=0; i<bufferSize_; ++i)
        {
            if (prngBitsUsed == 32)
            {
                prngWord = genrand_int32();
                prngBitsUsed = 0;
            }

            targetBuffer_[i] = targetsLfsrCorrector_.Next((char)((prngWord>>(i%32))&0x1));
            ++prngBitsUsed;
        }
    }
}


void PcqngAnnPrd::FillPrdBuffer()
{
    int prdBufferIndex = 0;
    for (int i=0; i<(bufferSize_/annStreamCount_); ++i)
    {
        // Get pcqng raw dequantized bits
        char rawOctet = -1;
        rng_->GetBuffer()->Read(&rawOctet, 1);
        
        // Corrector
        for (int j=0; j<annStreamCount_; ++j)
        {
            prdBuffer_[prdBufferIndex] = rngLfsrCorrector_.Next((rawOctet>>(j%7))&0x1);
            ++prdBufferIndex;
        }
    }
}


void PcqngAnnPrd::CompareTargetsFilter(double* prdStreams)
{
    int stepCount = 0;
    int streamsIndex = 0;
    for (int i=0; i<bufferSize_; ++i)
    {
        char comparedResult = 2 * (prdBuffer_[i] ^ !targetBuffer_[i]) - 1;
        prdStreams[streamsIndex] = compLpFilter_.Feed(comparedResult, compLpFilterConst_);
        if ((++stepCount % (bufferSize_/annStreamCount_)) == 0)
        {
            prdStreams[streamsIndex] /= 0.1414215;
            ++streamsIndex;
        }
    }
}

// Input z-score, returns cumulative normal distribution value
// Accuracy better than 1% to z=+/-7.5; .05% to z=+/-4.
double PcqngAnnPrd::ZToP(double zScore)
{
	double retval;

	// calculation variables
	double w;
	double y;
	double t;
	double num;
	double denom;

	// check
	if ( zScore > 8. )
	{
		zScore = 8.;
	}
	else 
	{
		if ( zScore < -8. )
			zScore = -8.;
	}


	// calculation constants
	double c[8];
	c[1] = 2.506628275;    c[2] = 0.31938153;     c[3] = -0.356563782;    c[4] = 1.781477937;
	c[5] = -1.821255978;   c[6] = 1.330274429;    c[7] = 0.2316419;

	w = (zScore>=0)? 1 : -1;
	t = 1. + (c[7]*w*zScore);
	y = 1./t;

	num = c[2] + (c[6] + (c[5]*t) + (c[4]*t*t) + (c[3]*t*t*t)) / (t*t*t*t) ;
	denom = c[1] * exp( .5*zScore*zScore ) * t;

	retval = 0.5 + w * ( .5 - (num/denom) );

	return retval;
}


void PcqngAnnPrd::ConvertZsToPsFilter(double* prdStreams)
{
    for (int i=0; i<annStreamCount_; ++i)
        prdStreams[i] = ZToP(prdStreams[i]);
}


int PcqngAnnPrd::PsComparator(const void* p1, const void* p2)
{
    double prob1 = *(double*)p1;
    double prob2 = *(double*)p2;
    if (prob1 == prob2)
        return 0;
    if (prob1 < prob2)
        return -1;
    else
        return 1;
}


void PcqngAnnPrd::SNormFilter(double* prdStreams)
{
    double smean[] = {.125, .25, .375, .5, .625, .75, .875};
    double sdev[] = {0.1102365, 0.1443345, 0.1613725, 0.1666667, 0.1613725, 0.1443345, 0.1102365};

    for (int i=0; i<annStreamCount_; ++i)
    {
        prdStreams[i] = (prdStreams[i]-smean[i])/sdev[i];
    }
}


void PcqngAnnPrd::FactorizeFilter(double* prdStreams)
{
    double eigv[] = {4.003734, 1.330968, 0.665942, 0.399806, 0.266232, 0.190674, 0.142645};
    double initEigTab[] =
        {0.288681, 0.378098, 0.422496, 0.436354, 0.422496, 0.378098, 0.288681,
        -0.500118, -0.436284, -0.244002, -0.000743, .244002, 0.436284, 0.500118,
        0.564627, 0.121894, -0.274793, -0.426195, -0.274793, 0.121894, 0.564627,
        -0.476458, 0.311638, 0.419355, 0.002797, -0.419355, -0.311638, 0.476458,
        0.309076, -0.541156, 0.030129, 0.470516, 0.030129, -0.541156, 0.309076,
        -0.151209, 0.46101, -0.51438, 0.003411, 0.51438, -0.46101, 0.151209,
        0.048236, -0.222086, 0.495051, -0.637607, 0.495051, -0.222086, 0.048236};
    double eigtab[7][7];
    memcpy(eigtab, initEigTab, 7*7*sizeof(double));


    double* prdStreamsCopy = new double[annStreamCount_];
    memcpy(prdStreamsCopy, prdStreams, annStreamCount_*sizeof(double));
    for (int i=0; i<annStreamCount_; ++i)
    {
        prdStreams[i] = 0;
        for (int j=0; j<annStreamCount_; ++j)
        {
            prdStreams[i] += prdStreamsCopy[j] * eigtab[i][j];
        }
        prdStreams[i] /= sqrt(eigv[i]);
    }
    delete[] prdStreamsCopy;
}


double* PcqngAnnPrd::GetLogStreams()
{
    return logStreams_;
}
