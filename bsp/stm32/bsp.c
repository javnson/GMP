// platform include file
#include <base/gmp_base.h>

// STM32 platform
#include "stm32l1xx_hal.h" 

void gmp_delay(gmp_size_t time)
{
	 HAL_Delay(time);
}
