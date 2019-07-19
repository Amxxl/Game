//
// IntegerTypes.h
//

#pragma once

using int8   = char;
using int16  = short;
using int32  = int;
using int64  = long long;

using sint8  = signed char;
using sint16 = signed short;
using sint32 = signed int;
using sint64 = signed long long;

using uint8  = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

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