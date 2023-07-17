
// System includes 
#include <base/gmp_base.h>


// This file provides two methods for saving the UUID	

#ifndef _FILE_UUID_H_
#define _FILE_UUID_H_

typedef struct _tag_uuid
{
	uint16_t uuid[8]; // 8 * 16 = 128
}uuid_t;

#define UUID_ZERO {0,0,0,0,0,0,0,0}


// the following is the functions which serves the uuid type

#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif


#endif // _FILE_UUID_H_
