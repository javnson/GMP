// This BSP file is for STM32 series devices

// platform include file
#include <base/gmp_base.h>

// STM32 platform
#include <bsp/hardware_relative_headers.h>

void gmp_delay(gmp_size_t time)
{
	 HAL_Delay(time);
}
