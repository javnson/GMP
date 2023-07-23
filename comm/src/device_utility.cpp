
// This file contains all the definition of all utility functions of device.

// EM-LIB basic objects
//#include <./comm/comm_basic.h>
#include "../comm_basic.h"
#include <./base/gmp_base.h>

#ifndef EM_DISABLE_DEVICE_UTILITY
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif // end of _DISABLE_UTILITY

// There are utility function definitions
#ifndef EM_DISABLE_DEVICE_UTILITY


gmp_diff_t put_format_string(io_device_base* device, gmp_addr_t addr, const char* p_fmt, ...)
{
	char str[SHOW_BUF_LEN];
	uint8_t len;

	va_list vArgs;
	va_start(vArgs, p_fmt);
	vsprintf((char*)str, (char const*)p_fmt, vArgs);
	va_end(vArgs);

	len = strlen(str);

	return device->write(addr, (gmp_data_t*)str, len);;
}




#endif // EM_DISABLE_DEVICE_UTILITY

