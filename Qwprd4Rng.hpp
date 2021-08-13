#ifndef INCLUDE_HPP_QWPRD4RNG_
#define INCLUDE_HPP_QWPRD4RNG_


#include "IRng.hpp"
#include <Windows.h>
#include <FTD2XX.h>
class IBuffer;


class Qwprd4Rng : public IRng
{
public:
    Qwprd4Rng(IBuffer* buffer);

public:
    virtual void Start();
    virtual void Stop();

private:
    HANDLE readerThread_;
    static unsigned __stdcall Reader(void* instance);
    HANDLE endReaderThreadSig_;

    FT_HANDLE ftHandle_;
};


#endif // INCLUDE_HPP_QWPRD4RNG_
