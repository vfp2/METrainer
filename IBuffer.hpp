#ifndef INCLUDE_HPP_IBUFFER_
#define INCLUDE_HPP_IBUFFER_


class IBuffer
{
public:
    virtual void Write(char* buffer, int bufferSize) = 0;
    virtual int Read(char* buffer, int bufferSize) = 0;
    virtual void Clear() = 0;
};


#endif // INCLUDE_HPP_IBUFFER_
