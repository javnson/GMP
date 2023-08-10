
#ifndef _FILE_CTRL_BASE_H_
#define _FILE_CTRL_BASE_H_

// control math computing definition
// Note: We should consider the following 3 libraries.
// IQ math is a outstanding fixed-pointing computing library,
// which is collection of highly-optimized and high-precision mathematical functions.
// This library is widely supported and applied in many scenarios.
// Some RISCV chips also support user to use IQ math, so this library is useful.
// So this is our beginning.
#ifdef CTRL_ENABLE_IQ_MATH

// Config IQ math lib
#ifndef GLOBAL_Q
#define GLOBAL_Q 24
#endif // GLOBAL_Q

//#define MATH_TYPE IQ_MATH

// invoke IQ math header
#include <IQmathLib.h>

#ifdef DISABLE_DEFAULT_IQMATH
// specify default control type
#define CTRL_TYPE _iq
#endif //end of DISABLE_DEFAULT_IQMATH
#endif // end of CTRL_ENABLE_IQ_MATH

// If the chip has a FPU, some mathematical computing may done more precisely.
// Thanks to the longer number of bits, 
// Floating-point numbers usually have higher precision.
#ifdef CTRL_ENABLE_FLOAT
// Enable Float-Point Unit don't means that user have to use float to complete all control computing.

#endif

// Common Micro-controller Software Interface Standard (CMSIS) 
// Majority of ARM chip support this library, because of DSP instruction set.
#ifdef CTRL_ENABLE_CMSIS_DSP
#include <arm_const_structs.h>
#include <arm_common_tables.h>
#include <arm_math.h>
#endif

// Mathematica library of GMP.
// GMP provide a set of high level math library.
#include <ctrl/math/gmp_math.h>

// include system math library.
// This library contains a variety of math functions, and ensure correct implementation.
#include <math.h>

// control type definition

// @brief control type is major type for control system.
// Major of computing routine will complete by this type.
// If the chip has a FPU you may use float instead of int32_t.
// If the chip has a IQ math lib, you may use _iq instead.
#ifndef CTRL_TYPE
typedef int32_t ctrl_t;
#else
typedef CTRL_TYPE ctrl_t;
#endif // end of CTRL_TYPE

// @brief Precision priority occasions.
// If the CPU has double precision FPU you may use double instead.
#ifndef CTRL_FLOAT_TYPE
typedef float ctrl_float_t;
#else 
typedef CTRL_FLOAT_TYPE ctrl_float_t;
#endif // end of CTRL_FLOAT_TYPE


//////////////////////////////////////////////////////////////////////////
// @brief inline function definition



//////////////////////////////////////////////////////////////////////////
// @brief Global control functions.
// This part of code is used to select default calculating function.

// Basic calculating functions

// Special functions
#define gmp_ctrl_sin sinf
#define gmp_ctrl_cos cosf
#define gmp_ctrl_exp expf
#define gmp_ctrl_log logf



// type conversion functions 



#endif // end of _FILE_CTRL_BASE_H_

