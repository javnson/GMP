// This file implement all the print relative functions.

// system includes
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// platform includes
#include <./basic/gmp_base.h>


__weak void gmp_dbg_write_default(_IN const gmp_data_t* content, _IN gmp_size_t len)
{
	return;
}



// implement of pointer
void (*dbg_write)(const gmp_data_t* content, gmp_size_t length) = gmp_dbg_write_default;

gmp_status_t gmp_print(_IN const gmp_data_t* content, ...)
{
	char str[SHOW_BUF_LEN];
	gmp_size_t len;

	va_list args;
	va_start(args, content);
	vsprintf((char*)str, (char const*)content, args);
	va_end(args);

	len = strlen(str);
	
	(*gmp_print)((const gmp_data_t*)str, len);

	return GMP_STATUS_OK;
}

uint16_t gmp_assert_print(_IN const gmp_data_t* msg, _IN const gmp_data_t* filepath, _IN gmp_size_t line)
{
	gmp_print(_TEXT("In file [%s:%ld] assert occured!\r\n[ERROR] %s"), filepath, line, msg);
	return 1;
}
