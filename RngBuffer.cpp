#include "RngBuffer.hpp"



RngBuffer::RngBuffer()
    : store_(new char[10000])
    , storeSize_(10000)
    , readMarker_(0)
    , writeMarker_(0)
{
    dataInStoreEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
    InitializeCriticalSection(&mutex_);
}


RngBuffer::~RngBuffer()
{
    CloseHandle(dataInStoreEvent_);
    DeleteCriticalSection(&mutex_);

    delete[] store_;
}


void RngBuffer::Write(char* buffer, int bufferSize)
{
    EnterCriticalSection(&mutex_);

    if ((writeMarker_ + bufferSize) > storeSize_)
    {
        int subFill = storeSize_-writeMarker_;
        memcpy(&store_[writeMarker_], buffer, subFill);
        writeMarker_ = 0;
        buffer = &buffer[subFill];
        bufferSize -= subFill;
    }

    memcpy(&store_[writeMarker_], buffer, bufferSize);
    writeMarker_ += bufferSize;

    SetEvent(dataInStoreEvent_);
    LeaveCriticalSection(&mutex_);
}


int RngBuffer::Read(char* buffer, int bufferSize)
{
    int bytesTxd = 0;
    while (bytesTxd < bufferSize)
    {
/*        if (writeMarker_ == readMarker_)
        {
            Sleep(0);
            continue;
        }
*/
        if (writeMarker_ == readMarker_)
        {
            if (WaitForSingleObject(dataInStoreEvent_, INFINITE) != WAIT_OBJECT_0)
                return -1;
        }

        EnterCriticalSection(&mutex_);

        if (writeMarker_ >= readMarker_)
        {
            int bytesAvailable = writeMarker_ - readMarker_;
            int bytesToTransfer = (bytesAvailable > (bufferSize-bytesTxd))? (bufferSize-bytesTxd) : bytesAvailable;
            memcpy(&buffer[bytesTxd], &store_[readMarker_], bytesToTransfer);
            readMarker_ += bytesToTransfer;
            bytesTxd += bytesToTransfer;
        }
        else
        {
            int bytesAvailable = storeSize_-readMarker_;
            int bytesToTransfer = (bytesAvailable > (bufferSize-bytesTxd))? (bufferSize-bytesTxd) : bytesAvailable;
            memcpy(&buffer[bytesTxd], &store_[readMarker_], bytesToTransfer);
            readMarker_ += bytesToTransfer;
            readMarker_ %= storeSize_;
            bytesTxd += bytesToTransfer;
        
            bytesAvailable = writeMarker_;
            bytesToTransfer = (bytesAvailable > (bufferSize-bytesTxd))? (bufferSize-bytesTxd) : bytesAvailable;
            memcpy(&buffer[bytesTxd], &store_[readMarker_], bytesToTransfer);
            readMarker_ += bytesToTransfer;
            bytesTxd += bytesToTransfer;
        }

        ResetEvent(dataInStoreEvent_);
        LeaveCriticalSection(&mutex_);
    }

    return 0;
}


void RngBuffer::Clear()
{
    EnterCriticalSection(&mutex_);

    readMarker_ = 0;
    writeMarker_ = 0;

    LeaveCriticalSection(&mutex_);
}
