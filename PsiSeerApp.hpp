#ifndef INCLUDE_HPP_PSISEERAPP_
#define INCLUDE_HPP_PSISEERAPP_


class PsiSeerGui;
class StatsGroup;
class IBuffer;
class IRng;
class IPrd;
class Logger;
class BoundedRandomWalk; // (NEW)


#include <Windows.h>
//#include "test/Threads.h"
//#include "Types.h"

typedef int (*FnPrdOpen)(char* licenseId);
typedef int (*FnPrdClose)(int prdHandle);
typedef int (*FnPrdGetData)(int prdHandle, int sampleCount, double* psiData, double* targetData);
typedef int (*FnPrdGetSource)(int prdHandle, int srcBuffLen, char* source);
typedef int (*FnSetPsiMode)(int prdHandle, int psiMode);
typedef int (*FnGetPsiMode)(int prdHandle);
typedef int (*FnPrdListSources)(int srcsBuffLen, char* sources);
typedef int (*FnPrdWrite)(int prdHandle, int outBuffLen, char* outBuffer);
typedef int (*FnPrdSetSource)(int prdHandle, char* source);
typedef int (*FnGetRawData)(int prdHandle, int count, char* rawData); // (NEW)
/*
typedef int (*FnStart)(char* userInterface, char* userId);
typedef int (*FnStop)();
typedef double (*FnGetPrdResult)();
typedef int (*FnSetPsiMode)(int psiMode);
typedef int (*FnSetMatchIntention)(int matchIntention);
typedef char* (*FnGetSource)();
*/
class PsiSeerApp
{
public:
    PsiSeerApp();
    ~PsiSeerApp();

public:
//    void CommandEvent();
    static unsigned __stdcall CommandThread(void* object);
    void ResetEvent();
    void PracticeEvent();
    void DataPump(char psiBit);
    void SetPsiModeEvent(int newPsiMode);
	void SetSource(char* source);
	static int Compare (const void * a, const void * b);

private:
    PsiSeerGui* gui_;

    StatsGroup* liveStats_;
    StatsGroup* practiceStats_;
    StatsGroup* currentStats_;

    LPVOID missWave_;
    LPVOID hitWave_;

    int psiMode_;
//    uint128 targetWord_;
	char targetWord_;	// (NEW)

    IBuffer* rngBuffer_;
    IRng* rng_;
    IPrd* prd_;

    IBuffer* hwRngBuffer_;
    IRng* hwRng_;
    IPrd* hwPrd_;

    HANDLE runnerThread_;
    HANDLE endEvent_;

public:
    HANDLE commandPulse_;
    Logger* logger_;
    bool logging_;
	char source_[20];
	bool isAc_;
	SOCKET outSock_;
    bool useSocketApi_;
    bool usingBrw_;
	int rawBufferSize_; // (NEW)

	BoundedRandomWalk* brw_; // (NEW)
};


#endif // INCLUDE_HPP_PSISEERAPP_
