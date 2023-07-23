// This header should only contains the hardware or the chip header.
#include <base/userconfig.h>

#ifdef _GMP_STM32L1
#include "stm32l1xx_hal.h"
#endif


// STM32 series devices
#ifdef _GMP_STM32
#ifdef _GMP_STM32L1
#include "stm32l1xx_hal.h"
#include <bsp/stm32/gpio_support.h>
#elif _GMP_STM32H7
#endif
#endif // end of _GMP_STM32 device series

// TMS320 series devices
#ifdef _GMP_C2000
#ifdef _GMP_28x
#include <DSP28x_Project.h>
#elif _GMP_280x

#endif 

#endif // end of _GMP_C2000 Device series
