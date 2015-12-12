/*********************************
**********************************
*****create by patrick at 2015/12/1***
**********************************
**********************************/

#ifndef COMMON_H_H_H
#define COMMON_H_H_H

#ifndef CUR_OS_LINUX
#define CUR_OS_WINDOWS
#endif

// Define cross platform variables 
#ifdef CUR_OS_LINUX

typedef char     int8_t;
typedef short int   int16_t;
typedef int        int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#else

typedef char     int8_t;
typedef short int   int16_t;
typedef int        int32_t;
typedef __int64 int64_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;


#endif






#endif
