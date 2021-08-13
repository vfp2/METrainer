//
// © 2006 Psigenics Corporation
//
// Types.h
//


#ifndef INCLUDE_H_TYPES_
#define INCLUDE_H_TYPES_


#ifdef __cplusplus
    extern "C"
    {
#endif


typedef signed char int8;
typedef unsigned char uint8;

typedef signed short int16;
typedef unsigned short uint16;

typedef signed long int32;
typedef unsigned long uint32;

typedef signed __int64 int64;
typedef unsigned __int64 uint64;

typedef struct
{
    unsigned __int64 Lo64;
    unsigned __int64 Hi64;
} uint128;

void Assign128(uint128* target128, uint64 hi64, uint64 lo64);
void BitShiftRight128(uint128* target128, unsigned order);
void BitShiftLeft128(uint128* target128, unsigned order);
void BitOr128(uint128* target128, uint128* source128);


#ifdef __cplusplus
    } // extern "C"
#endif


#endif // INCLUDE_H_TYPES_
