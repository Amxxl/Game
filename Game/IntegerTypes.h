//
// IntegerTypes.h
//

#pragma once

typedef char                  int8;
typedef short                 int16;
typedef int                   int32;
typedef long long             int64;

typedef signed char           sint8;
typedef signed short          sint16;
typedef signed int            sint32;
typedef signed long long      sint64;

typedef unsigned char         uint8;
typedef unsigned short        uint16;
typedef unsigned int          uint32;
typedef unsigned long long    uint64;

static_assert(sizeof(int8) == 1, "Size of int8 must be 1 byte alignment.");
static_assert(sizeof(int16) == 2, "Size of int16 must be 2 bytes alignment.");
static_assert(sizeof(int32) == 4, "Size of int32 must be 4 bytes alignment.");
static_assert(sizeof(int64) == 8, "Size of int64 must be 8 bytes alignment.");

static_assert(sizeof(sint8) == 1, "Size of sint8 must be 1 byte alignment.");
static_assert(sizeof(sint16) == 2, "Size of sint16 must be 2 bytes alignment.");
static_assert(sizeof(sint32) == 4, "Size of sint32 must be 4 bytes alignment.");
static_assert(sizeof(sint64) == 8, "Size of sint64 must be 8 bytes alignment.");

static_assert(sizeof(uint8) == 1, "Size of uint8 must be 1 byte alignment.");
static_assert(sizeof(uint16) == 2, "Size of uint16 must be 2 bytes alignment.");
static_assert(sizeof(uint32) == 4, "Size of uint32 must be 4 bytes alignment.");
static_assert(sizeof(uint64) == 8, "Size of uint64 must be 8 bytes alignment.");