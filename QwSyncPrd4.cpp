#include "QwSyncPrd4.hpp"
#include "FtdiRng.hpp"
#include "RngBuffer.hpp"
#include "MersenneTwister.hpp"
#include "stdio.h"
#include "math.h"

//FILE* theFile;

double netRuns[2];
double netHits[2][7];
char* netNames[] = {"s09", "s23", "s29", "s33", "s34", "s15", "s20"};
double bitHits[2];
double sevenHits[2];
double mmHits[2];


QwSyncPrd4::QwSyncPrd4(IRng* rng)
    : IPrd(rng)
    , logStreams_(new double[7*1995])
    , annA_(0)
    , annB_(0)
    , annC_(0)
    , annD_(0)
    , annE_(0)
    , annF_(0)
    , annG_(0)
    , medianA_(0.5)
    , medianB_(0.5)
    , medianC_(0.5)
    , medianD_(0.5)
    , medianE_(0.5)
    , medianF_(0.5)
    , medianG_(0.5)
{
    netRuns[0] = 0;
    netRuns[1] = 0;
    netDir = 1;
    netIndex = 0;
    memset(&netHits, 0, sizeof(double)*2*7);
    sevenHits[0] = 0;
    sevenHits[1] = 0;
    bitHits[0] = 0;
    bitHits[1] = 0;
    mmHits[0] = 0;
    mmHits[1] = 0;
//    theFile = fopen("CombOut.txt", "w+");

    for (int j=0; j<7; ++j)
        lpFilters_[j].Init(0);
    // look for backprop weights
    FILE* annFile = fopen("bootstrapS9_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annA_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annA_->restore("bootstrapS9_17.ann");
        annA_->alpha = alpha;
        annA_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianA_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }

        fclose(annFile);
    }

    annFile = fopen("bootstrapS23_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annB_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annB_->restore("bootstrapS23_17.ann");
        annB_->alpha = alpha;
        annB_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianB_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }

        fclose(annFile);
    }

    annFile = fopen("bootstrapS29_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annC_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annC_->restore("bootstrapS29_17.ann");
        annC_->alpha = alpha;
        annC_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianC_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }
        fclose(annFile);
    }

    annFile = fopen("bootstrapS33_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annD_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annD_->restore("bootstrapS33_17.ann");
        annD_->alpha = alpha;
        annD_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianD_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }
        fclose(annFile);
    }

    annFile = fopen("bootstrapS34_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annE_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annE_->restore("bootstrapS34_17.ann");
        annE_->alpha = alpha;
        annE_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianE_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }
        fclose(annFile);
    }

    annFile = fopen("bootstrapS15_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annF_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annF_->restore("bootstrapS15_17.ann");
        annF_->alpha = alpha;
        annF_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianF_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }
        fclose(annFile);
    }

    annFile = fopen("bootstrapS20_17.ann", "r");
    if (annFile != 0)
    {
        int layerCount = 0;
        fscanf(annFile, "%i", &layerCount);
        int* structure = new int[layerCount];
        for (int i=0; i<layerCount; ++i)
            fscanf(annFile, ", %i", &structure[i]);
        double beta, alpha;
        fscanf(annFile, "%lf, %lf", &beta, &alpha);
        annG_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annG_->restore("bootstrapS20_17.ann");
        annG_->alpha = alpha;
        annG_->beta = beta;

        // looking for threshold
        while (!feof(annFile))
        {
            int filePos = ftell(annFile);
            char line[41];
            fgets(line, 40, annFile);
            if (strchr(line, 'T') == line)
            {
                medianG_ = atof(&line[1]);
                break;
            }
            filePos = 0;
        }
        fclose(annFile);
    }

    if (annA_!=0 && annB_!=0 && annC_!=0 && annD_!=0 && annE_!=0)
        printf("Anns enabled!\n");
}


QwSyncPrd4::~QwSyncPrd4()
{
//    delete[] targetBuffer_;
//    delete[] prdBuffer_;
}

// Input z-score, returns cumulative normal distribution value
// Accuracy better than 1% to z=+/-7.5; .05% to z=+/-4.
double ZToP(double zScore)
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


double PToZ(double pValue)
{
    // fit constants
    double p[] = {-0.322232431088, -1.0, -0.342242088547, -0.0204231210245, -0.0000453642210148};
    double q[] = {0.099348462606, 0.588581570495, 0.531103462366, 0.10353775285, 0.0038560700634};

    double pp;
    if (pValue  < 0.5)
        pp = pValue;
    else
        pp = 1.0 - pValue;

    double y = sqrt(log(1.0/(pp*pp)));
    double retVal = y + ((((y*p[4] + p[3])*y + p[2])*y + p[1])*y + p[0]) / ((((y*q[4] + q[3])*y + q[2])*y + q[1])*y + q[0]);
    if (pValue < 0.5)
        retVal = -retVal;

    return retVal;
}


void ConvertZsToPsFilter(double* prdStreams)
{
    for (int i=0; i<7; ++i)
        prdStreams[i] = ZToP(prdStreams[i]);
}

void SNormFilter(double* prdStreams)
{
    double smean[] = {.125, .25, .375, .5, .625, .75, .875};
    double sdev[] = {0.1102397, 0.1443377, 0.1613745, 0.1666667, 0.1613745, 0.1443377, 0.1102397};

    for (int i=0; i<7; ++i)
    {
        prdStreams[i] = (prdStreams[i]-smean[i])/sdev[i];
    }
}


void FactorizeFilter(double* prdStreams)
{
    double eigv[] = {3.99988, 1.33345, 0.666633, 0.399989, 0.266686, 0.190498, 0.142862};
    double initEigTab[] =
        {0.28867, 0.377961, 0.422581, 0.436442, 0.422581, 0.377961, 0.28867,
        -0.499982, -0.436442, -0.244001, 0.000011, 0.244001, 0.436442, 0.499982,
        0.56412, 0.123018, -0.275204, -0.426376, -0.275204, 0.123018, 0.56412,
        -0.476755, 0.312085, 0.418697, 0.000041, -0.418697, -0.312085, 0.476755,
        0.310013, -0.541326, 0.030244, 0.468922, 0.030244, -0.541326, 0.310013, 
        -0.150738, 0.460567, -0.514933, 0.000017, 0.514933, -0.460567, 0.150738, 
        0.04828, -0.22131, 0.494746, -0.63862, 0.494746, -0.22131, 0.04828};
    double eigtab[7][7];
    memcpy(eigtab, initEigTab, 7*7*sizeof(double));


    double* prdStreamsCopy = new double[7];
    memcpy(prdStreamsCopy, prdStreams, 7*sizeof(double));
    for (int i=0; i<7; ++i)
    {
        prdStreams[i] = 0;
        for (int j=0; j<7; ++j)
        {
            prdStreams[i] += prdStreamsCopy[j] * eigtab[i][j];
        }
        prdStreams[i] /= sqrt(eigv[i]);
    }
    delete[] prdStreamsCopy;
}

int __cdecl TupleComparator(const void* p1, const void* p2)
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

double QwSyncPrd4::Generate()
{
    double aCoef[] = {0.0003938814457103237, 1.0445561820899898, -7.3933980723591475, 113.21151380015422, -886.6593452900868, 4346.87329699133, -14157.048935743227, 31276.4751236276, -46961.05352775994, 47130.93340914752, -30241.906246973285, 11214.68408146612, -1828.1615618222545};
    double bCoef[] = {0.008932042988488539, -0.19528595109302654, 48.98866088164961, -870.7412771620885, 7950.840072809116, -42273.103892322535, 140274.7530841425, -301915.31937204587, 428051.4454629876, -396651.50391088525, 230984.18598212206, -76626.28050570775, 11027.913857469512};
    double cCoef[] = {0.0021882553346299345, 0.9645069502445257, -4.2097969081947255, 55.77694268362243, -364.67954695905615, 1480.1723865795602, -3954.440529479002, 7095.942243831184, -8551.939629492168, 6772.925673795686, -3340.2672616507575, 913.2462959913237, -102.49583966578678};

    int superVoteWalk = 0;
    sevenHits[netIndex] = 0;
    for (int h=0; h<1995; ++h)
    {
        int j = 0;
        while (j<17)
        {
            static unsigned char randBytes[4];
            static int k = 0;
            if (k == 0)
            {
                rng_->GetBuffer()->Read((char*)randBytes, 4);
//                unsigned long prng32 = genrand_int32();
//                memcpy(randBytes, &prng32, 4);
                k = 4;
            }
            --k;

            unsigned char randByte = randBytes[3-k];
            for (int n=0; n<7; ++n)
            {
                lpFilters_[n].Feed(2*((randByte>>(6-n))&0x1) - 1, 10.508332);
                if ((randByte>>(6-n)&0x1) == 1)
                    bitHits[netIndex] += 1;
            }
            ++j;
        }

        int voteWalk = 0;
        double sevenTuple[7];
        for (int j=0; j<7; ++j)
        {
            // get normalized z
            sevenTuple[j] = lpFilters_[j].GetValue();
            sevenTuple[j] /= 0.2236068;

            // remember for log stream
            logStreams_[h*7 + j] = sevenTuple[j];
            lpFilters_[j].Init(0);

            if (sevenTuple[j] >= 0)
                ++voteWalk;
            else
                --voteWalk;
        }

        // Comvert from z-scores to p-values
        if ((annA_ != 0) && (annB_ != 0))
        {
            ConvertZsToPsFilter(sevenTuple);
            
            annA_->ffwd(sevenTuple);
            double resA = (annA_->Out(0)-medianA_)/0.116576;
//            resA = ZToP(resA);
            
            annC_->ffwd(sevenTuple);
            double out = annC_->Out(0);
            double resC = (out-medianC_)/0.088259;
//            resC = ZToP(resC);

            annG_->ffwd(sevenTuple);
            out = annG_->Out(0);
            double resG = (out-medianG_);

            // Sort ascending
            qsort((void*)sevenTuple, (size_t)7, sizeof(double), TupleComparator);
            // Normalize to SNorm
            SNormFilter(sevenTuple);
            // Factorize
            FactorizeFilter(sevenTuple);

            annB_->ffwd(sevenTuple);
            out = annB_->Out(0);
            double resB = (out-medianB_)/0.0799091;

            annF_->ffwd(sevenTuple);
            out = annF_->Out(0);
            double resF = (out-medianF_);

            ConvertZsToPsFilter(sevenTuple);

            annD_->ffwd(sevenTuple);
            double resD = (annD_->Out(0)-medianD_);
            annE_->ffwd(sevenTuple);
            double resE = (annE_->Out(0)-medianE_);
//            resB = ZToP(resB);
/*
            double fitA = 0;
            double fitB = 0;
            double fitC = 0;
            for (int n=0; n<=12; ++n)
            {
                fitA += aCoef[n] * pow(resA, (double)n);
                fitB += bCoef[n] * pow(resB, (double)n);
                fitC += cCoef[n] * pow(resC, (double)n);
            }
//            fprintf(theFile, "%1.16f\n", fitC);

            fitA += -0.000384271;
            fitB += -0.00205705;
            fitC += 0.00278458;
*/
//            printf("%f -> %f\n%f -> %f\n%f -> %f\n", resA, fitA, resB, fitB, resC, fitC);

//            fitA = PToZ(fitA);
//            fitB = PToZ(fitB);
//            fitC = PToZ(fitC);

/*            double final = fitA;
            if (fabs(final) < fabs(fitB))
                final = fitB;
//            if (fabs(final) < fabs(fitC))
//                final = fitC;

//            fprintf(theFile, "%1.16f %1.16f %1.16f\n", fitA, fitB, fitC);           

//            double final = (fitA + fitB/* + fitC)/sqrt(3.0);

//            fprintf(theFile, "%1.16f\n", final);
/*
            if (final >= 0.0)
                ++superVoteWalk;
            else
                --superVoteWalk;
  */         
            int sevenWalk = -superVoteWalk;
            ++netRuns[netIndex];
            if ((resA) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][0] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][0] -= netDir;
            }
            if ((resB) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][1] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][1] -= netDir;
            }
            if ((resC) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][2] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][2] -= netDir;
            }
/*            if ((resD) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][3] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][3] -= netDir;
            }*/
            if ((resE) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][4] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][4] -= netDir;
            }
            if ((resF) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][5] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][5] -= netDir;
            }
/*            if ((resG) >= 0.0)
            {
                ++superVoteWalk;
                netHits[netIndex][6] += netDir;
            }
            else
            {
                --superVoteWalk;
                netHits[netIndex][6] -= netDir;
            }*/
            sevenWalk += superVoteWalk;
            if (sevenWalk > 0)
                sevenHits[netIndex] += netDir;
            else
                sevenHits[netIndex] -= netDir;
        }
        else
        {
            if (voteWalk > 0)
                ++superVoteWalk;
            else
                --superVoteWalk;
        }
    }

    if (sevenHits[netIndex] > 0)
        ++mmHits[netIndex];

/*    system("cls");
    printf("\n Ann    N(+)       N(-)         N        HR(+)     HR(-)        HR      z(HR)\n\n");
    double allNetPos = 0;
    double allNetNeg = 0;
    double allNetPosN = 0;
    double allNetNegN = 0;
    for (int i=0; i<7; ++i)
    {
        double posHits = (netHits[0][i]+netRuns[0])/(2*netRuns[0]);
        allNetPos += netHits[0][i];
        allNetPosN += netRuns[0];
        if (netRuns[0] == 0)
            posHits = 0.5;
        double negHits = (netHits[1][i]+netRuns[1])/(2*netRuns[1]);
        allNetNeg += netHits[1][i];
        allNetNegN += netRuns[1];
        if (netRuns[1] == 0)
            negHits = 0.5;
        double totHits = ((posHits * netRuns[0]) + (negHits * netRuns[1])) / (netRuns[0] + netRuns[1]);
        double zHits = (2*totHits-1) * sqrt(netRuns[0] + netRuns[1]);
        printf(" %s  %0.2e  %0.2e  %0.2e   %01.6f  %01.6f   %01.6f  %+01.4f\n", netNames[i], netRuns[0], netRuns[1], netRuns[0]+netRuns[1], posHits, negHits, totHits, zHits);
    }
    printf(" -----------------------------------------------------------------------------\n");
    double aposHits = (allNetPos + allNetPosN) / (2*allNetPosN);
    if (allNetPosN == 0)
        aposHits = 0.5;
    double anegHits = (allNetNeg + allNetNegN) / (2*allNetNegN);
    if (allNetNegN == 0)
        anegHits = 0.5;
    double atotHits = ((aposHits * allNetPosN) + (anegHits * allNetNegN)) / (allNetPosN + allNetNegN);
    double azHits = (2*atotHits-1) * sqrt(allNetPosN + allNetNegN);
    printf(" %s  %0.2e  %0.2e  %0.2e   %01.6f  %01.6f   %01.6f  %+01.4f\n", "all", allNetPosN, allNetNegN, allNetPosN+allNetNegN, aposHits, anegHits, atotHits, azHits);
    printf(" -----------------------------------------------------------------------------\n");
    double bitNPos = allNetPosN*17;
    double bitNNeg = allNetNegN*17;
    double bitPosHits = bitHits[0]/bitNPos;
    if (bitNPos == 0)
        bitPosHits = 0.5;
    double bitNegHits = bitHits[1]/bitNNeg;
    if (bitNNeg == 0)
        bitNegHits = 0.5;
    double bitTotHits = ((bitPosHits * bitNPos) + (bitNegHits * bitNNeg)) / (bitNPos + bitNNeg);
    double bitZHits = (2*bitTotHits-1) * sqrt(bitNPos + bitNNeg);
    printf(" %s  %0.2e  %0.2e  %0.2e   %01.6f  %01.6f   %01.6f  %+01.4f\n", "bit", bitNPos, bitNNeg, (bitNNeg+bitNPos), bitPosHits, bitNegHits, bitTotHits, bitZHits);
    double mmNPos = netRuns[0]/463;
    double mmNNeg = netRuns[1]/463;
    double mmPosHits = mmHits[0]/mmNPos;
    if (mmNPos == 0)
        mmPosHits = 0.5;
    double mmNegHits = mmHits[1]/mmNNeg;
    if (mmNNeg == 0)
        mmNegHits = 0.5;
    double mmTotHits = ((mmPosHits * mmNPos) + (mmNegHits * mmNNeg)) / (mmNPos + mmNNeg);
    double mmZHits = (2*mmTotHits-1) * sqrt(mmNPos + mmNNeg);
    printf(" %s  %0.2e  %0.2e  %0.2e   %01.6f  %01.6f   %01.6f  %+01.4f\n", "463", mmNPos, mmNNeg, (mmNNeg+mmNPos), mmPosHits, mmNegHits, mmTotHits, mmZHits);
*/

    // return
    return (superVoteWalk > 0)? 1 : 0;
}


double* QwSyncPrd4::GetLogStreams()
{
    return logStreams_;
}


/*
void QwSyncPrd4::SetTargets()
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
*/
