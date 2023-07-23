// You shold only define macros in this file.
// You may find correct user config file in BSP folder.
// You may replace this file with what BSP provided.

#ifndef _FILE_USERCONFIG_EXAMPLE_H_
#define _FILE_USERCONFIG_EXAMPLE_H_

// Select DEVICE PLATFORM
#define _GMP_STM32L1


//////////////////////////////////////////////////////////////////////////
// Step 1 General Motor Platform setup config


//#define GMP_DEBUG


#define GMP_DATA_T uint8_t
#define gmp_data_s ((1))

// If you use CubeMX to generate initialization code automatically,
// or you write initialization code by your self
// Please uncommon this line of macro.
#define GMP_INIT_DEPOSIT 

// If you enable this macro when assert function are called, and the 
// condition couldn't meet, the program would abort.
#define DISABLE_GMP_ASSERT_ABORT

//////////////////////////////////////////////////////////////////////////
// Step 2 Disable or Enable some library



//////////////////////////////////////////////////////////////////////////
// Step 3 Add necessary headers here.



#endif //end of _FILE_USERCONFIG_EXAMPLE_H_
