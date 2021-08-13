//
// Copyright © 2012 Psigenics Corporation.
// 
// BoundedRandomWalk.hpp
//


#ifndef INCLUDE_HPP_BOUNDEDRANDOMWALK_
#define INCLUDE_HPP_BOUNDEDRANDOMWALK_


#include <stdexcept>
#include <math.h>
#include "Stats.hpp"

#define BIASMARKER 0x80	

class Stats;

class BoundedRandomWalk
{
public:
    BoundedRandomWalk(unsigned boundThreshold)
        : ret7Bits(0)
		, currentVal_(0)
		, currentPos_(0)
		, maxVal_(0)
		, minVal_(0)
		, maxPos_(0)
		, minPos_(0)
        , boundThreshold_(boundThreshold)
    {
        if (boundThreshold_ == 0)
            throw std::exception("Bound threshold must be non-zero");

		lut();	// create lookup table
    }

public:
	
	// Random Walk Process
	int feedRW(char* byteArray, int arraySize, int BlockSize, bool isAC)	// rawBuffer, returnBytes, blocksize = returnBytes/1429
	{

		for(int i=0; i<arraySize; i+=BlockSize)	
		{
			double ret7Bits = 0;
			
			// get count
			for(int j=0; j<BlockSize; j++) {
				if ((byteArray[i+j]&BIASMARKER) == BIASMARKER) 
					ret7Bits += ((double)bitCounter[(byteArray[i+j] & 0x7f)]);	// Bias Stream: get count from look up table
				else
					ret7Bits += ((double)bitCounter[byteArray[i+j]]);			// AC Stream: get count from look up table
			}

			ret7Bits /= sqrt((7.*(double)BlockSize));	// normalize by dividing by sqrt(#ofbits)
			currentVal_ += (double)ret7Bits;			// Random Walk with floating point
			currentPos_++;								// increment position

			if (currentVal_ > 0) {				// Find max/min values
				if(currentVal_ >= maxVal_) {
					maxVal_ = currentVal_;
					maxPos_ = currentPos_;
				}
			}
			else {
				if(currentVal_ <= minVal_) {
					minVal_ = currentVal_;
					minPos_ = currentPos_;
				}
			}
		}
/*
		for(int i=0; i<arraySize; i++)	
		{
			// Bias Stream
			if ((byteArray[i]&BIASMARKER) == BIASMARKER) 
			{
				ret7Bits = byteArray[i]&0x1;	// Shift and Strip Single Bits from 7-Bit raw word
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				 currentPos_++;						// increment location

				 if (currentVal_ > 0) {				// Find max/min values
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 }

				ret7Bits = (byteArray[i]>>1)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 }

				ret7Bits = (byteArray[i]>>2)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 }

				ret7Bits = (byteArray[i]>>3)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 }

				ret7Bits = (byteArray[i]>>4)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 }

				ret7Bits = (byteArray[i]>>5)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 }

				ret7Bits = (byteArray[i]>>6)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					 if(currentVal_ >= maxVal_) {
						 maxVal_ = currentVal_;
						 maxPos_ = currentPos_;
					 }
				 }
				 else {
					 if(currentVal_ <= minVal_) {
						 minVal_ = currentVal_;
						 minPos_ = currentPos_;
					 }
				 } 
			}
			else // AC Stream
			{
				ret7Bits = byteArray[i]&0x1;	// Shift and Strip Single Bits from 7-Bit raw word
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
					currentPos_++;						// increment location

					if (currentVal_ > 0) {				// Find max/min values
						if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
					}
					else {
						if(currentVal_ <= minVal_) {
							minVal_ = currentVal_;
							minPos_ = currentPos_;
						}
					}

				ret7Bits = (byteArray[i]>>1)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
						if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
					}
					else {
						if(currentVal_ <= minVal_) {
							minVal_ = currentVal_;
							minPos_ = currentPos_;
						}
					}

				ret7Bits = (byteArray[i]>>2)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
						if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
					}
					else {
						if(currentVal_ <= minVal_) {
							minVal_ = currentVal_;
							minPos_ = currentPos_;
						}
					}

				ret7Bits = (byteArray[i]>>3)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
						if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
					}
					else {
						if(currentVal_ <= minVal_) {
							minVal_ = currentVal_;
							minPos_ = currentPos_;
						}
					}

				ret7Bits = (byteArray[i]>>4)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
						if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
					}
					else {
						if(currentVal_ <= minVal_) {
							minVal_ = currentVal_;
							minPos_ = currentPos_;
						}
					}

				ret7Bits = (byteArray[i]>>5)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
						if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
					}
					else {
						if(currentVal_ <= minVal_) {
							minVal_ = currentVal_;
							minPos_ = currentPos_;
						}
					}

				ret7Bits = (byteArray[i]>>6)&0x1;	
				if (ret7Bits == 1)
					++currentVal_;
				else
					--currentVal_;
				currentPos_++;

				if (currentVal_ > 0) {				
					if(currentVal_ >= maxVal_) {
							maxVal_ = currentVal_;
							maxPos_ = currentPos_;
						}
				}
				else {
					if(currentVal_ <= minVal_) {
						minVal_ = currentVal_;
						minPos_ = currentPos_;
					}
				}

			}
			// check if block ended
			if(currentPos_==BlockSize)
				break;
		}
*/
		return currentPos_;
	}

	// Get Terminal Random Walk count
	double getResult() 
	{
		double z = 0.0;
		double p = 0.0;

		// find szcore for N0 -> Nf
		double z0f_ = ((double)currentVal_)/sqrt((double)currentPos_);
		/***** DEBUG *****/
		std::cout << "\n\nN0 -> Nf Z : " << z0f_ << "\n\n";

		// find szcore for N+- -> N-+
		double ppm_ = 0.0;
		double zpm_ = 0.0;
		if (maxPos_ > minPos_) {	// (Z+)
			z = ((double)(maxVal_ - minVal_))/sqrt((double)(maxPos_ - minPos_));	
			if (z==0)
				zpm_ = 0.0;
			else {
				p = Stats::ZToP(z);	
				ppm_ = pow(p,(double)currentPos_/(double)(maxPos_ - minPos_)); // find adjusted p: p' = (p)^(N/N')
				zpm_ = Stats::PToZ(ppm_); 
				zpm_ = (zpm_ < 0) ? 0.0 : zpm_;
			}
		}
		else {	// (Z-)
			z = ((double)(minVal_ - maxVal_))/sqrt((double)(minPos_ - maxPos_));
			if (z==0)
				zpm_ = 0.0;
			else {
				p = Stats::ZToP(abs(z));	
				ppm_ = pow(p,(double)currentPos_/(double)(minPos_ - maxPos_)); // find adjusted p: padj = 1 - (1 - p')^(N/N')
				zpm_ = Stats::PToZ(ppm_); 
				zpm_ = (zpm_ < 0) ? 0.0 : -1.*zpm_;
			}
		}
		/***** DEBUG *****/
		std::cout << "Max Peak / Position      : " << maxVal_ << " / " << maxPos_ << '\n';
		std::cout << "Min Peak / Position      : " << minVal_ << " / " << minPos_ << '\n';
		std::cout << "Window N                 : " << currentPos_ << '\n';
		std::cout << "Npm -> Nmp Z   : " << z << '\n';
		std::cout << "Npm -> Nmp p   : " << p << '\n';
		std::cout << "Npm -> Nmp Padj: " << ppm_ << '\n';
		std::cout << "Npm -> Nmp Zadj: " << zpm_ << "\n\n";

		// find zscore for N0 -> N+ (Z+)
		double z0p_ = 0.0;
		double p0p_ = 0.0;
		z = (maxPos_==0) ? 0.0 : ((double)maxVal_)/sqrt((double)maxPos_);
		if (z==0)
			z0p_ = 0.0;
		else {
			p = Stats::ZToP(z);	
			p0p_ = pow(p,(double)currentPos_/(double)maxPos_); // find adjusted p: p' = (p)^(N/N')
			z0p_ = Stats::PToZ(p0p_); 
			z0p_ = (z0p_ < 0) ? 0.0 : z0p_;
		}
		/***** DEBUG *****/
		std::cout << "Max Peak / Position      : " << maxVal_ << " / " << maxPos_ << '\n';
		std::cout << "Window N                 : " << currentPos_ << '\n';
		std::cout << "N0 -> Np Z     : " << z << '\n';
		std::cout << "N0 -> Np P     : " << p << '\n';
		std::cout << "N0 -> Np Padj  : " << p0p_ << '\n';
		std::cout << "N0 -> Np Zadj  : " << z0p_ << "\n\n";

		// find zscore for N+ -> Nf (Z-)
		double zpf_ = 0.0;
		double ppf_ = 0.0;
		z = (maxPos_==currentPos_) ? 0.0 : ((double)(currentVal_ - maxVal_))/sqrt((double)(currentPos_ - maxPos_));
		if (z==0)
			zpf_ = 0.0;
		else {
			p = Stats::ZToP(abs(z));	
			ppf_ = pow(p,(double)currentPos_/(double)(currentPos_ - maxPos_)); // find adjusted p: padj = 1 - (1 - p')^(N/N')
			zpf_ = Stats::PToZ(ppf_); 
			zpf_ = (zpf_ < 0) ? 0.0 : -1.*zpf_;
		}
		/***** DEBUG *****/
		std::cout << "Max Peak / Position      : " << maxVal_ << " / " << maxPos_ << '\n';
		std::cout << "Window N                 : " << currentPos_ << '\n';
		std::cout << "Np -> Nf Z     : " << z << '\n';
		std::cout << "Np -> Nf P     : " << p << '\n';
		std::cout << "Np -> Nf Padj  : " << ppf_ << '\n';
		std::cout << "Np -> Nf Zadj  : " << zpf_ << "\n\n";

		// find zscore for N0 -> N- (Z-)
		double z0m_ = 0.0;
		double p0m_ = 0.0;
		z = (minPos_==0) ? 0.0 : ((double)minVal_)/sqrt((double)minPos_);
		if (z==0)
			z0m_ = 0.0;
		else {
			p = Stats::ZToP(abs(z));	
			p0m_ = pow(p,(double)currentPos_/(double)minPos_); // find adjusted p: padj = 1 - (1 - p')^(N/N')
			z0m_ = Stats::PToZ(p0m_); 
			z0m_ = (z0m_ < 0) ? 0.0 : -1.*z0m_;
		}
		/***** DEBUG *****/
		std::cout << "Min Peak / Position      : " << minVal_ << " / " << minPos_ << '\n';
		std::cout << "Window N                 : " << currentPos_ << '\n';
		std::cout << "N0 -> Nm Z     : " << z << '\n';
		std::cout << "N0 -> Nm P     : " << p << '\n';
		std::cout << "N0 -> Nm Padj  : " << p0m_ << '\n';
		std::cout << "N0 -> Nm Zadj  : " << z0m_ << "\n\n";

		// find zscore for N- -> Nf (Z+)
		double zmf_ = 0.0;
		double pmf_ = 0.0;
		z = (minPos_==currentPos_) ? 0.0 : ((double)(currentVal_ - minVal_))/sqrt((double)(currentPos_ - minPos_));
		if (z==0)
			zmf_ = 0.0;
		else {
			p = Stats::ZToP(z);	
			pmf_ = pow(p,(double)currentPos_/(double)(currentPos_ - minPos_)); // find adjusted p: p' = (p)^(N/N')
			zmf_ = Stats::PToZ(pmf_); 
			zmf_ = (zmf_ < 0) ? 0.0 : zmf_;
		}
		/***** DEBUG *****/
		std::cout << "Min Peak / Position      : " << minVal_ << " / " << minPos_ << '\n';
		std::cout << "Window N                 : " << currentPos_ << '\n';
		std::cout << "Nm -> Nf Z     : " << z << '\n';
		std::cout << "Nm -> Nf P     : " << p << '\n';
		std::cout << "Nm -> Nf Padj  : " << pmf_ << '\n';
		std::cout << "Nm -> Nf Zadj  : " << zmf_ << "\n\n";
		
		// return largest absolute value (keep sign)
		double zVal = 0;
		zVal = (abs(z0f_)>abs(zpm_))? z0f_ : zpm_;
		zVal = (abs(zVal)>abs(z0p_))? zVal : z0p_;
		zVal = (abs(zVal)>abs(zpf_))? zVal : zpf_;
		zVal = (abs(zVal)>abs(z0m_))? zVal : z0m_;
		zVal = (abs(zVal)>abs(zmf_))? zVal : zmf_;

		if (zVal == 0 )
			zVal = (ret7Bits == 0x1) ? 0.0001 : -0.0001;

		/***** DEBUG *****/
		std::cout << "zVal           : " << zVal << '\n';

		double retVal = 0;
		retVal = pFit(zVal);	// get probability of the “z-scores” from the “best” of random walk probabilities

		/***** DEBUG *****/
		std::cout << "retVal         : " << retVal << '\n';

		// Reset values
		Reset();

		return retVal;
	}

	// Calculates probability of the “z-scores” from the “best” of random walk probabilities
	double pFit( double zval ) 
	{	
		double z;
		double p;
		double retVal;
	
		// check boundaries
		if (abs(zval) >= 4.0) {
			if (zval < 0 ) 
				return -3.100;	// p = 0.000967
			else
				return 3.100;	// p = 0.999033
		}

		if (zval<0)
			z = abs(zval);
		else
			z = zval;

		p = ( 0.5 - (2.23158285389073e-2 * pow(z,3)) - (6.01428118682251e-2 * pow(z,4)) + (0.103097361979695 * pow(z,5)) + (9.67222543396317e-2 * pow(z,6)) - (0.12744720245611 * pow(z,7)) - (1.7415572365306e-2 * pow(z,8)) + 
		(7.12182928223516e-2 * pow(z,9)) - (3.11748482255755e-2 * pow(z,10)) - (8.82430001769106e-4 * pow(z,11)) + (5.50720387242436e-3 * pow(z,12)) - (2.33141533432108e-3 * pow(z,13)) + (5.13366328895632e-4 * pow(z,14)) - 
		(6.69099152725947e-5 * pow(z,15)) + (5.05794534035813e-6 * pow(z,16)) - (1.93235497252082e-7 * pow(z,17)) + (2.37684409220248e-9 * pow(z,18)) );

		if (zval<0)
			retVal = Stats::PToZ(1-p);
		else
			retVal = Stats::PToZ(p);

		return retVal;
	}

	// Reset current random walk position
    void Reset()
    {
		currentVal_ = 0;
		currentPos_ = 0;

		maxVal_ = 0;
		minVal_ = 0;
		maxPos_ = 0;
		minPos_ = 0;
    }

	void lut()	// (NEW)
	{
		char numOnes = 0; 
		char numZeros = 0;
		char *ptr;
		ptr = bitCounter;
		char inbyte = 0x00;
		for(int k=0; k<128; k++)
		{
			for(int BitCounter=0; BitCounter<7; ++BitCounter) // Loop 7-Bit raw word
			{
				char retbit=(inbyte>>(char)BitCounter)&0x1;		// Strip Single Bits from 7-Bit raw word
				if(retbit==1)	// compare bit
					numOnes += 1;
				else
					numZeros += 1;
			}
			*ptr = numOnes - numZeros;	
			ptr++;
			inbyte++;
			numOnes = 0;
			numZeros = 0;
		}
	}

private:
	char ret7Bits;
	char bitCounter[128];	// (NEW)

    double currentVal_;
	int currentPos_;

	double maxVal_;
	double minVal_;
	int maxPos_;
	int minPos_;

    unsigned boundThreshold_;
};


#endif // INCLUDE_HPP_BOUNDEDRANDOMWALK_
