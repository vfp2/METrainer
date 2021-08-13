#ifndef INCLUDE_HPP_RNGBUFFER_
#define INCLUDE_HPP_RNGBUFFER_


#include "IBuffer.hpp"
#include <Windows.h>


class RngBuffer : public IBuffer
{
public:
    RngBuffer();
    ~RngBuffer();

public:
    virtual void Write(char* buffer, int bufferSize);
    virtual int Read(char* buffer, int bufferSize);
    virtual void Clear();

private:
    char* store_;
    int storeSize_;
    int writeMarker_;
    int readMarker_;

    HANDLE dataInStoreEvent_;
    CRITICAL_SECTION mutex_;
};


#endif // INCLUDE_HPP_RNGBUFFER_
