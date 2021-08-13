#ifndef INCLUDE_HPP_IRNG_
#define INCLUDE_HPP_IRNG_


class IBuffer;


class IRng
{
public:
    IRng(IBuffer* buffer);

public:
    virtual void Start();
    virtual void Stop();
    IBuffer* GetBuffer();
    virtual void SetMode(int mode);
    virtual int GetMode();

protected:
    IBuffer* buffer_;
    int mode_;
};


#endif // INCLUDE_HPP_IRNG_
