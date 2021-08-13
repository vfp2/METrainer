//#include "PrdSelector.hpp"

#include "MersenneTwister.hpp"

#include "PsiSeerApp.hpp"
#include "PsiSeerGui.hpp"
#include "StatsGroup.hpp"
#include <FL/Fl.H>
#include <iostream>
#include <math.h>
#include "resource.h"
#include "time.h"
#include <direct.h>
#include "Types.h"
#include "Logger.hpp"
//#include "LpFilter.hpp"

#include <list>

#include "BoundedRandomWalk.hpp" // (NEW)

#include "PsiLogger.h"

#define PRD_CORE_PATH_ "PrdCore.dll"
#import "QWQNG.dll" no_namespace named_guids

#define BIASMARKER 0x80	
#define BIASMASK   0x7F	

FnPrdOpen PrdOpen;
FnPrdClose PrdClose;
FnPrdGetData GetPrdData;
FnPrdGetSource PrdGetSource;
FnSetPsiMode SetPsiMode;
FnGetPsiMode GetPsiMode;
FnPrdListSources PrdListSources;
FnPrdWrite PrdWrite;
FnPrdSetSource PrdSetSource;
FnGetRawData GetRawData; // (NEW)

int prdHandle;

int test = 1;

#define PSISEERAPP_STATSWIN_LEN 41

CRITICAL_SECTION critX2_;

double netRuns[2];
double netHits[2][20];


inline char NextBit()
{
    static unsigned long word = 0;
    static int bitCount = 0;

    if (bitCount == 0)
    {
        word = genrand_int32();

        bitCount = 32;
    }

    return (char)(word >> (--bitCount))&0x1;
}

int main()
{
    InitializeCriticalSection(&critX2_);

    LARGE_INTEGER seed;
    QueryPerformanceCounter(&seed);
    init_genrand(seed.LowPart);

    PsiSeerApp theApp;

    // Import PrdCore DLL
    HINSTANCE prdCore = LoadLibraryA(PRD_CORE_PATH_);
    PrdOpen = (FnPrdOpen)GetProcAddress(prdCore, "Open");
    PrdClose = (FnPrdClose)GetProcAddress(prdCore, "Close");
    GetPrdData = (FnPrdGetData)GetProcAddress(prdCore, "GetPrdData");
    GetPsiMode = (FnGetPsiMode)GetProcAddress(prdCore, "GetPsiMode");
    SetPsiMode = (FnSetPsiMode)GetProcAddress(prdCore, "SetPsiMode");
    PrdGetSource = (FnPrdGetSource)GetProcAddress(prdCore, "GetSource");
	PrdSetSource = (FnPrdSetSource)GetProcAddress(prdCore, "SetSource");
    PrdListSources = (FnPrdListSources)GetProcAddress(prdCore, "ListSources");
	PrdWrite = (FnPrdWrite)GetProcAddress(prdCore, "Write");
	GetRawData = (FnGetRawData)GetProcAddress(prdCore, "GetRawData"); // (NEW)

    int sourceNameLen = 10;
    int maxSourceCount = 20;
    char* sources = (char*)malloc(maxSourceCount * sourceNameLen);

    // Read and list all available sources
    int sourceCount = PrdListSources(sizeof(sources), sources);

	LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON),IMAGE_ICON,0,0,LR_DEFAULTSIZE); // (NEW)

	return Fl::run();    
}

//FILE* theFile;

PsiSeerApp::PsiSeerApp()
    : gui_(0)
    , useSocketApi_(false)
    , liveStats_(new StatsGroup(PSISEERAPP_STATSWIN_LEN))
    , practiceStats_(new StatsGroup(PSISEERAPP_STATSWIN_LEN))
    , psiMode_(1)
    , runnerThread_(0)
    , endEvent_(CreateEvent(NULL, TRUE, FALSE, NULL))
    , commandPulse_(CreateEvent(NULL, FALSE, FALSE, NULL))
    , logging_(true)
	, isAc_(false)
{
    currentStats_ = liveStats_;
    gui_ = new PsiSeerGui(this);

//    PRD_SetSource("Twister");
//    PRD_GetSource();

//    gui_->SetSource();

    for (int i=0; i<2; ++i)
    {
        netRuns[i] = 0;
        for (int j=0; j<20; ++j)
            netHits[i][j] = 0;
    }
    currentStats_->Reset();
    gui_->SetWinStats(currentStats_->GetWindowHitRate(), currentStats_->GetWindowPValue());
    gui_->SetTrialStats(currentStats_->GetTrialTotal(), currentStats_->GetTrialHitRate(), currentStats_->GetTrialPValue());

    ::ResetEvent(endEvent_);
//    Sleep(800);
    runnerThread_ = (HANDLE)_beginthreadex(NULL, 0, PsiSeerApp::CommandThread, this, 0, NULL);
}


PsiSeerApp::~PsiSeerApp()
{
    if (useSocketApi_ == true)
	    closesocket(outSock_);
//    fclose(theFile);
    SetEvent(endEvent_);
    SetEvent(commandPulse_);
    WaitForSingleObject(runnerThread_, INFINITE);
    CloseHandle(runnerThread_);
    CloseHandle(endEvent_);
    CloseHandle(commandPulse_);

    PrdClose(prdHandle);
//    BitBuffer_Destroy(psiBits_);
//    BitBuffer_Destroy(targetBits_);
    if (gui_ != 0) { delete gui_; gui_ = 0; }
	if (brw_ != 0) { delete brw_; brw_ = 0; }	// (NEW)
}

//void PsiSeerApp::CommandEvent()
static LARGE_INTEGER perfStart, perfEnd, perfTicks;
unsigned __stdcall PsiSeerApp::CommandThread(void* object)
{
    // Reset prd (buffer) and get result
    PsiSeerApp* inst = static_cast<PsiSeerApp*>(object);
    /*
    SOCKET outSock;
    if (inst->useSocketApi_ == true) {
	    sockaddr_in dest;
	    sockaddr_in local;
	    WSAData data;
	    WSAStartup( MAKEWORD( 2, 2 ), &data );

	    local.sin_family = AF_INET;
	    local.sin_addr.s_addr = INADDR_ANY;
	    local.sin_port = 0; // choose any

	    dest.sin_family = AF_INET;
	    dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	    dest.sin_port = htons(4903);

	    // create the socket
	    outSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	    // bind to the local address
    //	bind(outSock_, (sockaddr*)&local, sizeof(local));
	    connect(outSock, (sockaddr*)&dest, sizeof(dest));
    }*/
	// send the pkt
//	int ret = sendto( s, pkt, pkt_length, 0, (sockaddr *)&dest, sizeof(dest) );

	char outData[1024];

    while (1)
    {
        // is it time to end this thread?
        if (WaitForSingleObject(inst->endEvent_, 0) == WAIT_OBJECT_0)
            break;

		DWORD waitResponse = WaitForSingleObject(inst->commandPulse_, INFINITE);
		if (waitResponse == WAIT_OBJECT_0)
		{

			inst->gui_->ShowBusy();

			LARGE_INTEGER seed;	// (NEW)

			double psiData[80];
			double targetData[40];
			double streamData[5];
			double prdResult = 0;

			/* Timer Debug */
			/*LARGE_INTEGER  start;			// performance counter 
			LARGE_INTEGER  stop;			// performance counter
			LARGE_INTEGER proc_freq;		// performance counter Frequency
			double elapsedTime = 0;
			
			QueryPerformanceCounter(&start); // start timer
			QueryPerformanceCounter(&stop); // stop timer
			QueryPerformanceFrequency(&proc_freq);	// get processor frequency
			elapsedTime +=  (((stop.QuadPart - start.QuadPart) / (double)proc_freq.QuadPart))*1000;	
			std::cout << "Elapsed Time: " << elapsedTime << " ms\n";			*/

			/******************************** 
			*		Generate Output			*
			********************************/
			if (inst->usingBrw_ == true)	// Bounded Random Walk Processing
			{
				int j = 0;
				int blockSize;		
				bool haveResult = false;
				char* rawBuffer;
				int bufferSize;
	
				// Determine buffer size.
				if(inst->isAc_==true) {
					//blockSize = ((inst->rawBufferSize_)/1435)*14;	// AC Stream
                    blockSize = (inst->rawBufferSize_*14)/1435;	// AC Stream
					bufferSize = inst->rawBufferSize_*2;
				}
				else {
                    blockSize = (inst->rawBufferSize_*7)/1435;	// Bias Strea,\m
					bufferSize = inst->rawBufferSize_;
				}

				rawBuffer = new char [bufferSize];	// create storage for Prd Data

				while(!haveResult) {
					memset(rawBuffer, 0, inst->rawBufferSize_*sizeof(char));
					int returnBytes = GetRawData(prdHandle, bufferSize*sizeof(char), rawBuffer);
					int BitCounter = inst->brw_->feedRW(rawBuffer, returnBytes, blockSize, inst->isAc_);		// return current bit count
					streamData[j] = inst->brw_->getResult();	// random walker Zscore
					j++;
					if(j==5) {
						haveResult=true;
						break;
					}
				}

				delete [] rawBuffer;

				// PC psiMode: Request random target bits
				if(inst->psiMode_==3)	
				{
					//inst->targetWord_ = genrand_int32() & 0xff;
					QueryPerformanceCounter(&seed);
					srand(seed.LowPart);
					inst->targetWord_ = rand() % 0xff;
				}

				// Compare Psi/Target Bits
				double prdScore = 0;
				double targetbit = 0;

				for (int i=0; i<5; ++i) {
					if(abs(streamData[i]) > 0.) {
						if (inst->psiMode_==1) { // if PK psiMode
							prdScore += streamData[i];	
						}
						else {
							targetbit= (((inst->targetWord_>>i)&0x1) > 0)? 1 : -1;
							streamData[i] *= targetbit;
            				prdScore += streamData[i];
						}
					}
				}

				/***** DEBUG *****/
				std::cout << "\nFinal Zscore         : " << prdScore/sqrt(5.) << '\n';

				// Set default if prdScore is zero
				if(prdScore == 0) {
					double tmpStreamData[5];
					memcpy(tmpStreamData,streamData,sizeof(streamData));
					qsort(tmpStreamData,5,sizeof(double),&PsiSeerApp::Compare);
					prdScore = tmpStreamData[0];
				}

				prdResult = (prdScore > 0)? 1 : -1;

				// CV psiMode: Request random target bits for next trial
				if(inst->psiMode_==2)	
				{
					//inst->targetWord_ = genrand_int32() & 0xff;
					QueryPerformanceCounter(&seed);
					srand(seed.LowPart);
					inst->targetWord_ = rand() % 0xff;
				}
			}
			else	// combZ ANN Processing
			{
				double zSum = 0;
				for (int m=0; m<5; ++m)
				{
					double prdScore = 0;
                    GetPrdData(prdHandle, 40, psiData, targetData);
                    for (int i=0; i<40; ++i) {
                        if (inst->isAc_ == false)
                            prdScore += psiData[i]*targetData[i];
                        else
                            prdScore += psiData[i+40]*targetData[i];
                    }
					//prdResult += (prdScore > 0)? 1 : -1;
					//streamData[m] = prdScore/sqrt(40.0);
					streamData[m] = prdScore/sqrt(40.0);
					if(abs(streamData[m]) > 1.) {
						zSum += streamData[m];	
					}
				}

				if(zSum == 0) {
					double tmpStreamData[5];
					memcpy(tmpStreamData,streamData,sizeof(streamData));
					qsort(tmpStreamData,5,sizeof(double),&PsiSeerApp::Compare);
					zSum = tmpStreamData[0];
				}

				prdResult = (zSum > 0)? 1 : -1;
			}	
			/**************************************** 
			*		Log Data and Update Screen		*
			****************************************/
			double timeNow = Logger::TimeNowUtc();
			if (inst->logging_ == true)
				inst->logger_->LogPoint(timeNow, (inst->gui_->IsTargetInverted()==true)?(prdResult > 0)?0:1:(prdResult > 0)?1:0, (inst->gui_->IsTargetInverted()==true)?0:1, inst->psiMode_, 5, streamData);
			if (inst->useSocketApi_ == true) {
				sprintf(outData, "%08.0f, %10.3f, %i, %i, %i, %+1.5f, %+1.5f, %+1.5f, %+1.5f, %+1.5f, %s", inst->currentStats_->GetTrialTotal()+1, timeNow, (inst->gui_->IsTargetInverted()==true)?(prdResult > 0)?0:1:(prdResult > 0)?1:0, (inst->gui_->IsTargetInverted()==true)?0:1, inst->psiMode_, streamData[0], streamData[1], streamData[2], streamData[3], streamData[4], inst->source_);
				send(inst->outSock_, outData, strlen(outData), 0);
			}

			prdResult = (inst->gui_->IsTargetInverted()==true)? (prdResult>0)?0:1 : (prdResult>0)?1:0;
			if (prdResult > 0)
			{
				inst->currentStats_->Feed(1);
				if (!inst->gui_->IsMuted())
				{
					if (!inst->gui_->IsTargetInverted())
						sndPlaySound("Media/Sounds/HitChord.wav", SND_ASYNC | SND_FILENAME);
					else
						sndPlaySound("Media/Sounds/MissChord.wav", SND_ASYNC | SND_FILENAME);
				}
				inst->gui_->ShowHit();
			}
			else // Miss
			{
				inst->currentStats_->Feed(0);
		//        if (!inst->gui_->IsPracticeMode())
		//            PsiLogger_Log(PsiLogger_CreateS2(0, targeted, inst->psiMode_, logStreams));
				if (!inst->gui_->IsMuted() && !inst->gui_->IsMissMuted())
				{
					if (!inst->gui_->IsTargetInverted())
                			sndPlaySound("Media/Sounds/MissChord.wav", SND_ASYNC | SND_FILENAME);
					else
            			sndPlaySound("Media/Sounds/HitChord.wav", SND_ASYNC | SND_FILENAME);
				}
				if (inst->gui_->IsMissVisible())
        			inst->gui_->ShowMiss();
			}

			double windowPValue = inst->currentStats_->GetWindowPValue();
			double trialPValue = inst->currentStats_->GetTrialPValue();
			inst->gui_->SetWinStats(inst->currentStats_->GetWindowHitRate(), windowPValue);
			inst->gui_->SetTrialStats(inst->currentStats_->GetTrialTotal(), inst->currentStats_->GetTrialHitRate(), trialPValue);
    
			double lesserPValue = (windowPValue < trialPValue)? windowPValue : trialPValue;
			if (inst->currentStats_->GetTrialTotal() < PSISEERAPP_STATSWIN_LEN)
				lesserPValue = windowPValue;
			inst->gui_->SetSlider(log(1./lesserPValue)/log(2.));

		/*    EnterCriticalSection(&critX2_);
			commandEventRunning_ = false;
			LeaveCriticalSection(&critX2_);
		*/
		}
    }
    return 0;
}


void PsiSeerApp::ResetEvent()
{
    currentStats_->Reset();

    double windowPValue = currentStats_->GetWindowPValue();
    double trialPValue = currentStats_->GetTrialPValue();
    gui_->SetWinStats(currentStats_->GetWindowHitRate(), windowPValue);
    gui_->SetTrialStats(currentStats_->GetTrialTotal(), currentStats_->GetTrialHitRate(), trialPValue);

    double greaterPValue = (windowPValue > trialPValue)? windowPValue : trialPValue;
    gui_->SetSlider(log(1./greaterPValue)/log(2.));

    for (int i=0; i<2; ++i)
    {
        netRuns[i] = 0;
        for (int j=0; j<20; ++j)
            netHits[i][j] = 0;
    }

	// if CV PsiMode, get target for first trial 
	if(psiMode_==2)		
	{
		//targetWord_ = genrand_int32() & 0xff;
		LARGE_INTEGER seed;
		QueryPerformanceCounter(&seed);
		srand(seed.LowPart);
		targetWord_ = rand() % 0xff;
	}

	// If socket networking selected
	if (useSocketApi_ == true) {
		char outData[1024];
		sprintf(outData, "%s", "reset");
		send(outSock_, outData, strlen(outData), 0);
	}
}

void PsiSeerApp::PracticeEvent()
{
    if (gui_->IsPracticeMode())
    {
        currentStats_ = practiceStats_;
        ResetEvent();
    }
    else
    {
        currentStats_ = liveStats_;

        double windowPValue = currentStats_->GetWindowPValue();
        double trialPValue = currentStats_->GetTrialPValue();
        gui_->SetWinStats(currentStats_->GetWindowHitRate(), windowPValue);
        gui_->SetTrialStats(currentStats_->GetTrialTotal(), currentStats_->GetTrialHitRate(), trialPValue);
    
        double greaterPValue = (windowPValue > trialPValue)? windowPValue : trialPValue;
        gui_->SetSlider(log(1./greaterPValue)/log(2.));
    }
}

void PsiSeerApp::SetPsiModeEvent(int newPsiMode)
{
    psiMode_ = newPsiMode;

//    PRD_SetMode(newPsiMode);
//    if (gui_->IsFeedbackInverted())
//        PRD_SetIntention(0);
//    else
//        PRD_SetIntention(1);

    ResetEvent();
}

void PsiSeerApp::SetSource(char* source) {
	PrdSetSource(prdHandle, source);
	strcpy(source_, source);
}

int PsiSeerApp::Compare (const void * a, const void * b)
{
	double aa = abs(*(double*)a);
	double bb = abs(*(double*)b);
	if (aa<bb)
		return 1;
	if(aa>bb)
		return -1;

	return 0;
}