#pragma once

#include <math.h>

//#include "PreCogLib.h"

// number of simultaneous dimensions
#define DIMENSIONS 1

// number of codes in the codespace, be sure the codes_ array is appropriately
// initialized for the desired number of codes
#define NUMCODES 2

// bit length of the code, including any error control bits
#define CODELEN 1

// number of server bits to vote for each code bit
#define BITVOTE 5

// number of post-voted bits, set equal to codelen for ML and SINGLE 
// type decodes, set to desired window length for SLIDING type decode
#define INPLEN  1

// number of code words to pass into ML type decoding
#define CWORDS 1

// bit hit-rate assumption for calculating ML type decode probabilities
#define BHR 0.53

// maximum matching distance allowed in sliding window type decode
#define MAXDIST 1

// Decode types: ML = maximum likelihood, Sliding = sliding window, single = single bit
#define SLIDING 1
#define ML 2
#define SINGLE 3
#define DECODE_TYPE SINGLE

// window size for sliding window switching type application
#define WINDOWSIZE 9

#if CODELEN == 31
	static long codes_[NUMCODES] = {0x00000000, 0x00007FFF, 0x7FFF8000, 0x7FFFFFFF};
#elif	CODELEN == 17
	static long codes_[NUMCODES] = {0x00000, 0x000FF, 0x1FF00, 0x1FFFF};
#elif CODELEN == 9
	// 9 bit codes
//	static long codes_[NUMCODES] = {0x1AA,0x16A,0x0DC,0x03F};
	static long codes_[NUMCODES] = {0x000,0x00F,0x1F0,0x1FF};
#elif CODELEN == 8
	// 8 bit codes
//	static long codes_[10] = {0x00,0xF0,0xCC,0xA6,0x96,0x69,0x6A,0x33,0x0F,0xFF};
#elif CODELEN == 7
	// 7 bit codes
//	static long codes_[10] = {0x00,0x70,0x4C,0x26,0x16,0x69,0x5A,0x33,0x0F,0x7F};
	static long codes_[NUMCODES] = {0x69,0x4E,0x27,0x1B};
#elif CODELEN == 5
	// 5 bit codes
	static long codes_[10] = {0x00,0x1C,0x13,0x09,0x05,0x1A,0x16,0x0C,0x03,0x1F};
#elif CODELEN == 2
	static long codes_[NUMCODES] = {0x0,0x1,0x2,0x3};
#elif CODELEN == 1
	static long codes_[NUMCODES] = {0x0,0x1};
#endif


class CPsiStats
{
unsigned char new_pred_, predHist_[WINDOWSIZE], predIndex_;
int hits0_, hits1_, trials_, predictions_, predHits_, predOnes_, predZeros_, keyStrokes_;
int trials1_, trials0_, windowHits_;
long double pHits_, pVote_, hitPercent_, hitBalance_,pPreds_,zPreds_,QFactor_,predBalance_;
long double predQ_,predXY_[DIMENSIONS];
float probabilities_[CODELEN];

public:

	CPsiStats(void)
	{
		init_stats();
	}

	~CPsiStats(void)
	{
	}

	void init_stats(void)
	{
		int i;

		for(i=0;i<CODELEN;i++)
		{
			probabilities_[i] = (float)(pow(BHR,(CODELEN-i))*pow(1.0-BHR,i));
		}
		for(i=0;i<DIMENSIONS;i++)
		{
			predXY_[i]=0.0;
		}

		for(i=0;i<WINDOWSIZE;i++)
		{
			predHist_[i]=2;
		}
		windowHits_=0;
		hits0_=0;
		hits1_=0;
		trials_=0;
		trials1_=0;
		trials0_=0;
		predictions_=0;
		predHits_=0;
		hitPercent_=0.0;
		hitBalance_=0.5;
		pHits_=0.0;
		pVote_=0.0;
		pPreds_=0.0;
		zPreds_=0.0;
		QFactor_=0.0;
		new_pred_=1;
		predIndex_=0;
		predOnes_=0;
		predZeros_=0;
		predQ_=0.5;
		keyStrokes_=0;
	}

	void resetPredStats(void)
	{
		int i;

		for(i=0;i<WINDOWSIZE;i++)
		{
			predHist_[i]=2;
		}
		windowHits_=0;
		predictions_=0;
		predHits_=0;
		predOnes_=0;
		trials0_=0;
		trials1_=0;
		predZeros_=0;
		pPreds_=0.0;
		zPreds_=0.0;
		predBalance_=0.5;
		predQ_=0.5;
		keyStrokes_=0;
		for(i=0;i<DIMENSIONS;i++)
		{
			predXY_[i]=0.0;
		}
	}

/*	unsigned char doMLDecode(long *result)
	{
		char winner,distance[CWORDS],i,k;
		char choice;
		float P_winner, P_tmp;

		winner=0;
		P_winner=0.0;
		for(i=0;i<NUMCODES;i++)
		{
			P_tmp=0.0;
			for(k=0;k<CWORDS;k++)
			{
				distance[k] = hammingDist(result[k], codes_[i], CODELEN);			
				P_tmp += probabilities_[distance[k]];
			}
			if(P_tmp == P_winner)
			{
				PreCog_GetBit(&choice);
				if(choice == 1)
				{
					winner=i;
					P_winner = P_tmp;
				}
			}
			else if(P_tmp > P_winner)
			{
				winner=i;
				P_winner = P_tmp;
			}
		}

		return winner;
	}
*/
	unsigned char doSlidingDecode(long int input)
	{
		char i,j,distance;

		for(j=0;j<=(INPLEN-CODELEN);j++)
		{
			for(i=0;i<NUMCODES;i++)
			{
				distance=hammingDist((input>>j), codes_[i], CODELEN);
				if(distance <= MAXDIST)
					return i;
			}
		}

		return 99;
	}

	char hammingDist(long pattern1, long pattern2, char patternLen)
	{
		int i;
		long tmp;
		char dist;

		tmp=pattern1^pattern2;
		dist=0;
		for(i=0;i<patternLen;i++)
		{
			dist += (char)(tmp&0x01);
			tmp = tmp>>1;
		}

		return dist;
	}

	void updatePredXY(char *result, char isHit)
	{
		long double tmp[DIMENSIONS];
		int i;

		for(i=0;i<DIMENSIONS;i++)
		{
			tmp[i]=predXY_[i]*(long double)predictions_;
			tmp[i]+= result[i];
			predXY_[i]= tmp[i]/(long double)(predictions_ + 1);
		}
		if(isHit)
			record_prediction(0,0,0);
		else
			record_prediction(0,1,0);

	}

	void record_prediction(char target, char winner, char LT_Pred)
	{
		new_pred_=1;
// method 2, pass in winning bit as argument
		if(target == 0)
			trials0_++;
		else
			trials1_++;
		if(winner < 2)
		{
			++predictions_;
			if(target==winner)
			{
				predHist_[predIndex_]=1;
				predHits_++;
				if(LT_Pred==1)
					++predOnes_;
				else
					++predZeros_;
			}
			else
			{
				predHist_[predIndex_]=0;
			}
			predIndex_ = (predIndex_+1)%WINDOWSIZE;
	// method 1, get winner internally
	/*		if(getWinner() < 2)
				++predictions_;
			if(target==getWinner())
				predHits_++;
	*/
			
			if(predHits_ > 0)
				predBalance_ = fabs(((long double)predOnes_/(long double)predHits_)-0.5)+0.5;
			else
				predBalance_ = 0.0;
//			zPreds_ = z_score(predHits_,keyStrokes_);
			zPreds_ = z_score(predHits_,predictions_);
			pPreds_ = p_vote(predBalance_,predHits_);
//			predQ_ = (ZtoP(zPreds_)*(2.0*pPreds_ -1.0)+1.0)/2.0;
		}
	}

	void begin_prediction(void)
	{
/*		for(i=0;i<WINDOWSIZE;i++)
		{
			predHist_[i]=2;
		}
		predIndex_=0;
*/
		hits0_=0;
		hits1_=0;
		trials_=0;
		hitPercent_=0.0;
		hitBalance_=0.5;
		pHits_=0.0;
		pVote_=0.0;
		QFactor_=0.5;
		new_pred_=0;
	}

	char recordTrial(char hit, char result)
	{
		if(new_pred_==1)
		{
			begin_prediction();
		}
		trials_++;
		keyStrokes_++;
		if(hit==1) 
		{
			result==1 ? hits1_++ : hits0_++;
//			predHist_[predIndex_]=result;
//			predIndex_ = (predIndex_+1)%WINDOWSIZE;
		}
		hitBalance_ = (hits1_ + hits0_ == 0) ? 0.0 : 0.5+fabs((hits1_/(long double)(hits1_+hits0_))-0.5);
		hitPercent_ = (long double)(hits1_+hits0_)/trials_;
		pHits_ = p_hits(hits0_, hits1_);
		pVote_ = p_vote(hitBalance_,hits0_+hits1_);
		QFactor_=(pHits_*(2.0*pVote_ -1.0)+1.0)/2.0;
		
		if(hit)
			return getWinner();
		else
			return 2;
	}
	
	int getWindowResult(void)
	{
		int i,count,run;

		count=0;
		run=0;
		// run through circular buffer starting with oldest entry
		i=predIndex_;
		do 
		{
			if(predHist_[i] == 1)
			{
				count++;
				run++;
			}
			else
				run=0;
			i=(i+1)%WINDOWSIZE;
			if(run >= 6)
			{
				break;
			}
		}while(i!=predIndex_);

		// check for "hit" conditions
//		if(count >= 8 || run >= 6)
		if(run >= 6)
		{
			// flush the history
			for(i=0;i<WINDOWSIZE;i++)
			{
				predHist_[i] = 2;
			}
			predIndex_ = 0;
			windowHits_++;
			// return value indicates a "hit"  
			return 1;
		}
		else
			// return value indicates a "miss"
			return 0;

	}

	int getWindowHits(void)
	{
		return windowHits_;
	}

	float get0PredHR(void)
	{
		return trials0_ == 0 ? 0.500F : (float)predZeros_ / (float)trials0_;
//		return trials0_ == 0 ? 0.500 : (float)2*predZeros_ / (float)predictions_;
	}

	float get1PredHR(void)
	{
		return trials1_ == 0 ? 0.500F : (float)predOnes_ / (float)trials1_;
//		return trials1_ == 0 ? 0.500 : (float)2*predOnes_ / (float)predictions_;
	}

	int getHits(void)
	{
		return hits1_+hits0_;
	}

	int getTrials(void)
	{
		return trials_;
	}

	int getPredictions(void)
	{
		return predictions_;
	}

	int getPredHits(void)
	{
		return predHits_;
	}

	long double getPredBalance(void)
	{
		return predBalance_;
	}

	long double getPredHitRate(void)
	{
//		return keyStrokes_ == 0 ? 0.0 : (long double)predHits_ / (long double)keyStrokes_;
		return predictions_ == 0 ? 0.500 : (long double)predHits_ / (long double)predictions_;
	}

	unsigned char getPredWinner(void)
	{
		if(predZeros_ != predOnes_)
			return predOnes_ > predZeros_ ? 1 : 0;
		else
			return 2;
	}

	long double getPredQ(void)
	{
		return predQ_;
	}

	long double getPredX(void)
	{
		return predXY_[0];
	}

	long double getPredY(void)
	{
		return predXY_[1];
	}

	unsigned char getWinner(void)
	{
// method 4, winner is majority vote, vote count determined by MainDlg
		return hits1_ > hits0_ ? 1 : 0;

// method 3, winner every keypress, majority vote 2 out of 3 done in MainDlg
//		return predHist_[0];

// method 2, winner is run of two bits
/*		if(predHist_[0] == predHist_[1])
		{
			return predHist_[0];
		}
		else
			return 2;
*/
// old method, winner is majority vote winner		
/*		if(hits1_ > hits0_)
			return 1;
		else if(hits0_ > hits1_)
			return 0;
		else
			return 2;
*/
	}

	long double getPredBinomial(void)
	{
		return predHits_ == 0 ? 0.0 : betai(predHits_,predictions_-predHits_+1,1.0/pow((float)NUMCODES,(float)DIMENSIONS)); 
	}

	long double get_pVote(void)
	{
		return pVote_;
	}

	long double get_pHits(void)
	{
		return pHits_;
	}

	long double get_pHR(void)
	{
		long double tmp;

		tmp = ZtoP(-fabs(zPreds_));
		if(tmp < 0.0)
			return 0.0;
		else
			return tmp;
	}

	long double get_pPreds(void)
	{
		return pPreds_;
	}

	long double get_zPreds(void)
	{
		return zPreds_;
	}

	long double getHitPercent(void)
	{
		return hitPercent_;
	}

	long double getHitBalance(void)
	{
		return hitBalance_;
	}

	long double getQFactor(void)
	{
		return QFactor_;
	}

	static long double p_vote(long double pR, int r)
	{
		int n;
		long double result,lpr,lpr_inv;

		if(pR>=1.0)
			return 1.0;
		else if(r < 1)
			return 0.5;
		else if(r%2==0)
			return (3.0*p_vote(pR,r-1)+6.0*p_vote(pR,r+1)-p_vote(pR,r+3))/8.0;
		else
		{
			result=0.0;
			lpr=log(pR);
			lpr_inv=log(1.0-pR);
			for(n=(int)ceil((r+1.0)/2.0);n<=r;n++)
			{
				result+=exp(n*lpr+(r-n)*lpr_inv+lnbin(r,n));
			}
			return result;
		}
	}

	// log factorial
	static long double factln(int n)
	{
		return gammln((long double)n+1.0);
	}

	//incomplete beta function for calculating cumulative binomial probability
	static long double betai(long double a, long double b, long double x)
	{
		long double bt;

        if (a == 0.)
            return 1.0;

		if(x<0.0 || x>1.0)
			return 0.0;

		if(x==0.0 || x==1.0) bt=0.0;
		else
			bt=exp(gammln(a+b)-gammln(a)-gammln(b)+a*log(x)+b*log(1.0-x));
		if(x < (a+1.0)/(a+b+2.0))
			return bt*betacf(a,b,x)/a;
		else
			return 1.0-bt*betacf(b,a,1.0-x)/b;
	}

	//continued fraction evaluation for incomplete beta function
#define MAXIT 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30
	static long double betacf(long double a, long double b, long double x)
	{
		int m,m2;
		long double aa,c,d,del,h,qab,qam,qap;

		qab=a+b;
		qap=a+1.0;
		qam=a-1.0;
		c=1.0;
		d=1.0-qab*x/qap;
		if(fabs(d) < FPMIN) d=FPMIN;
		d=1.0/d;
		h=d;
		for(m=1; m<=MAXIT; m++)
		{
			m2=2*m;
			aa=m*(b-m)*x/((qam+m2)*(a+m2));
			d=1.0+aa*d;
			if(fabs(d) < FPMIN) d=FPMIN;
			c=1.0+aa/c;
			if(fabs(c) < FPMIN) c=FPMIN;
			d=1.0/d;
			h *= d*c;
			aa= -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
			d=1.0+aa*d;
			if(fabs(d) < FPMIN) d=FPMIN;
			c=1.0+aa/c;
			if(fabs(c) < FPMIN) c=FPMIN;
			d=1.0/d;
			del=d*c;
			h *= del;
			if(fabs(del-1.0) < EPS) break;
		}
		if(m > MAXIT){} //too many iterations, return exception value?
		return h;
	}

	// log of gamma function
	static long double gammln(long double xx)
	{
		long double x,y,tmp,ser;
		static long double cof[6]={76.18009172947146,-86.50532032941677,24.01409824083091,
			-1.231739572450155,0.1208650973866179e-2,-0.5395239384953e-5};
		int j;

		y=x=xx;
		tmp=x+5.5;
		tmp -= (x+0.5)*log(tmp);
		ser=1.000000000190015;
		for (j=0; j<=5; j++) ser += cof[j]/++y;
		return -tmp+log(2.5066282746310005*ser/x);
	}

	// log of binomial combination function
	static long double lnbin(int n, int k)
	{
//		static int nlast=0;
//		static long double fact_const, log_const;

//		if(nlast!=n)
//		{
//			fact_const=factln(n);
//			log_const=(0.5+n)*log((long double)n);
//			nlast=n;
//		}

		if(n<=k)
			return 0.0;
		if(k <1000 || (n-k) < 1000)
			return factln(n)-factln(k)-factln(n-k);
//			return fact_const-factln(k)-factln(n-k);
		else 
//			return log_const-(0.5+k)*log((long double)k)
			return (0.5+n)*log((long double)n)-(0.5+k)*log((long double)k)
				-(0.5-k+n)*log((long double)(n-k))-0.9189385332046727;
	}

	static float z_score(int h, int n)
	{
		return (float)((n<=1 || h>n) ? 0.0 : (2.0*h-(float)n)/sqrt((float)n));
	}

	// 2-tailed probability, higher of h0 or h1 
	static float p_hits(int h0, int h1)
	{
		int h_tot;
		float result;

		h_tot=h0+h1;
		result = (float)((h0==h1 && h0==0) ? 0.0 : 1.0-2.0*ZtoP(-fabs(z_score(h0,h_tot))));
		if(result < 0.0) result = 0.0;

		return result;
	}

	// Input z-score, returns cumulative normal distribution value
	// Accuracy better than 1% to z=+/-7.5; .05% to z=+/-4.
	static double ZtoP( double zScore )
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

};
