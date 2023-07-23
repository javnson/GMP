// This BSP file is for TMS320C28x series devices

// platform include file
#include <base/gmp_base.h>

// DSP platform
#include <bsp/hardware_relative_headers.h>

void gmp_delay(gmp_size_t time)
{
	HAL_Delay(time);
}

