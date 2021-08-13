#include "Types.h"


void Assign128(uint128* target128, uint64 hi64, uint64 lo64)
{
    target128->Hi64 = hi64;
    target128->Lo64 = lo64;
}


void BitShiftRight128(uint128* target128, unsigned order)
{
    if (order > 64)
    {
        target128->Lo64 = target128->Hi64>>(order-64);
        target128->Hi64 = 0;
    }
    else
    {
        target128->Lo64 >>= order;
        target128->Lo64 |= (target128->Hi64<<(64-order));
        target128->Hi64 >>= order;
    }
}


void BitShiftLeft128(uint128* target128, unsigned order)
{
    if (order >= 64)
    {
        target128->Hi64 = target128->Lo64<<(order-64);
        target128->Lo64 = 0;
    }
    else
    {
        target128->Hi64 <<= order;
        target128->Hi64 |= (target128->Lo64>>(64-order));
        target128->Lo64 <<= order;
    }
}

void BitOr128(uint128* target128, uint128* source128)
{
    target128->Lo64 |= source128->Lo64;
    target128->Hi64 |= source128->Hi64;
}
