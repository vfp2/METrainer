#ifndef INCLUDE_HPP_FTDIRNG_
#define INCLUDE_HPP_FTDIRNG_


#include "IRng.hpp"
#include <Windows.h>
#include <FTD2XX.h>
class IBuffer;


class FtdiRng : public IRng
{
public:
    FtdiRng(IBuffer* buffer);

public:
    virtual void Start();
    virtual void Stop();

private:
    HANDLE readerThread_;
    static unsigned __stdcall Reader(void* instance);
    HANDLE endReaderThreadSig_;

    FT_HANDLE ftHandle_;
};


#endif // INCLUDE_HPP_FTDIRNG_
