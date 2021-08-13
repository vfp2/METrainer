#include "PcqngSyncPrd.hpp"
#include "IRng.hpp"
#include "IBuffer.hpp"
#include "memory.h"
#include "MersenneTwister.hpp"


PcqngSyncPrd::PcqngSyncPrd(IRng* rng)
    : IPrd(rng)
    , logStreams_(new double[7*13])
    , annA_(0)
    , annB_(0)
    , annC_(0)
    , annD_(0)
    , annE_(0)
    , medianA_(0.5)
    , medianB_(0.5)
    , medianC_(0.5)
    , medianD_(0.5)
    , medianE_(0.5)
{
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
        annD_ = new CBackProp(layerCount, structure, beta, alpha);
        delete[] structure;
        annD_->restore("bootstrapS15_17.ann");
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

    if (annA_!=0 && annB_!=0 && annC_!=0 && annD_!=0 && annE_!=0)
        printf("Anns enabled!\n");
}


extern double ZToP(double zScore);
extern void ConvertZsToPsFilter(double* prdStreams);
extern void SNormFilter(double* prdStreams);
extern void FactorizeFilter(double* prdStreams);
extern int __cdecl TupleComparator(const void* p1, const void* p2);


double PcqngSyncPrd::Generate()
{
    double aCoef[] = {0.0003938814457103237, 1.0445561820899898, -7.3933980723591475, 113.21151380015422, -886.6593452900868, 4346.87329699133, -14157.048935743227, 31276.4751236276, -46961.05352775994, 47130.93340914752, -30241.906246973285, 11214.68408146612, -1828.1615618222545};
    double bCoef[] = {0.008932042988488539, -0.19528595109302654, 48.98866088164961, -870.7412771620885, 7950.840072809116, -42273.103892322535, 140274.7530841425, -301915.31937204587, 428051.4454629876, -396651.50391088525, 230984.18598212206, -76626.28050570775, 11027.913857469512};
    double cCoef[] = {0.0021882553346299345, 0.9645069502445257, -4.2097969081947255, 55.77694268362243, -364.67954695905615, 1480.1723865795602, -3954.440529479002, 7095.942243831184, -8551.939629492168, 6772.925673795686, -3340.2672616507575, 913.2462959913237, -102.49583966578678};

    int superVoteWalk = 0;
    for (int h=0; h<13; ++h)
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
                lpFilters_[n].Feed(2*((randByte>>(6-n))&0x1) - 1, 10.508332);
            ++j;
        }

        int voteWalk = 0;
        double sevenTuple[7];
        for (int j=0; j<7; ++j)
        {
            // get normalized z
            sevenTuple[j] = lpFilters_[j].GetValue();
            sevenTuple[j] /= .2236068;

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

            // Sort ascending
            qsort((void*)sevenTuple, (size_t)7, sizeof(double), TupleComparator);
            // Normalize to SNorm
            SNormFilter(sevenTuple);
            // Factorize
            FactorizeFilter(sevenTuple);

            annB_->ffwd(sevenTuple);
            out = annB_->Out(0);
            double resB = (out-medianB_)/0.0799091;

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
            
            if ((resA) >= 0.0)
                ++superVoteWalk;
            else
                --superVoteWalk;
            if ((resB) >= 0.0)
                ++superVoteWalk;
            else
                --superVoteWalk;
            if ((resC) >= 0.0)
                ++superVoteWalk;
            else
                --superVoteWalk;
            if ((resD) >= 0.0)
                ++superVoteWalk;
            else
                --superVoteWalk;
            if ((resE) >= 0.0)
                ++superVoteWalk;
            else
                --superVoteWalk;
        }
        else
        {
            if (voteWalk > 0)
                ++superVoteWalk;
            else
                --superVoteWalk;
        }
    }

    // return
    return (superVoteWalk > 0)? 1 : 0;
}

double* PcqngSyncPrd::GetLogStreams()
{
    return logStreams_;
}
