#include "FtdiRng.hpp"
#include "IBuffer.hpp"
#include <process.h>
#include "stdio.h"


#define FTDIB_DEVICE_LATENCY_MS 4
#define FTDIB_DEVICE_PACKET_USB_SIZE 192


FtdiRng::FtdiRng(IBuffer* buffer)
    : IRng(buffer)
    , ftHandle_(0)
{}


void FtdiRng::Start()
{
    // already open?
    if (ftHandle_ != 0)
        return;

    // search for an available device
    DWORD ftdiDevices = 0;
    if (FT_ListDevices(&ftdiDevices, NULL, FT_LIST_NUMBER_ONLY) != FT_OK)
        return;
    char serialNum[12];
    for (DWORD devIndex=0; devIndex<ftdiDevices; ++devIndex)
    {

        FT_ListDevices((PVOID)devIndex, serialNum, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
        if ((memcmp(serialNum, "QWP", 3) == 0) || (memcmp(serialNum, "QWR", 3) == 0))
        {
            if (FT_OpenEx((PVOID)serialNum, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle_) == FT_OK)
                break;
            ftHandle_ = 0;
        }
    }
    if (ftHandle_ == 0)
        throw "NO HARDWARE"; // did not find an available device

    // ftdi B series tweak
    if (memcmp(serialNum, "QWP", 3) == 0)
    {
		FT_SetLatencyTimer(ftHandle_, FTDIB_DEVICE_LATENCY_MS);
		FT_SetUSBParameters(ftHandle_, FTDIB_DEVICE_PACKET_USB_SIZE, FTDIB_DEVICE_PACKET_USB_SIZE);
    }

	// Start the device
	DWORD bytesWritten = 0;
	char prdStartCommand[2] = {0x01, 0x00};
	if (FT_Write(ftHandle_, prdStartCommand, 2, &bytesWritten) != FT_OK)
	{
		Stop();
		throw "NO HARDWARE";
	}
	if (bytesWritten != 2)
	{
		Stop();
		throw "NO HARDWARE";
	}
	DWORD rxBytes, txBytes, eventStatus;
	if (FT_GetStatus(ftHandle_, &rxBytes, &txBytes, &eventStatus) != FT_OK)
	{
		Stop();
		throw "NO HARDWARE";
	}

    // start the reader thread
    endReaderThreadSig_ = CreateEvent(NULL, TRUE, FALSE, 0);
    readerThread_ = (HANDLE)_beginthreadex(NULL, 0, FtdiRng::Reader, this, 0, NULL);
}


void FtdiRng::Stop()
{
    // end reader thread
    SetEvent(endReaderThreadSig_);
    WaitForSingleObject(readerThread_, INFINITE);
    CloseHandle(readerThread_);
    CloseHandle(endReaderThreadSig_);

    // close hardware
    FT_Close(ftHandle_);
    ftHandle_ = 0;
}


unsigned __stdcall FtdiRng::Reader(void* instance)
{
    // boost the priority of the hardware
    SetThreadAffinityMask(GetCurrentThread(), 1);
//    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    FtdiRng* inst = (FtdiRng*)instance;

    DWORD bytesReturned = 0;
    char buffer[FTDIB_DEVICE_PACKET_USB_SIZE];

    while (WaitForSingleObject(inst->endReaderThreadSig_, 0) != WAIT_OBJECT_0)
    {
        FT_Read(inst->ftHandle_, buffer, FTDIB_DEVICE_PACKET_USB_SIZE, &bytesReturned);
        inst->buffer_->Write(buffer, bytesReturned);
    }

    return 0;
}
