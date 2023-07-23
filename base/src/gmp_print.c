// This file implement all the print relative functions.

// system includes
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// platform includes
#include <base/gmp_base.h>


__weak void gmp_dbg_write_default(_IN const char* content, _IN gmp_size_t len)
{
	return;
}


// implement of pointer
void (*dbg_write)(const char* content, gmp_size_t length) = gmp_dbg_write_default;


__weak gmp_stat_t gmp_print(_IN const char* content, ...)

{
	char str[SHOW_BUF_LEN];
	gmp_size_t len;

	va_list args;
	va_start(args, content);
	vsprintf((char*)str, (char const*)content, args);
	va_end(args);

	len = strlen(str);
	
	(*dbg_write)(str, len);

	return GMP_STAT_OK;

}

__weak uint16_t gmp_assert_print(_IN const char* msg, _IN const char* filepath, _IN gmp_size_t line)
{
	g_info_cnt += 1;

	gmp_print(_TEXT("[ASSERT.%d]In file [%s:%ld] assert occurred!\r\n[ERROR] %s"),g_info_cnt, filepath, line, msg);

#ifndef DISABLE_GMP_ASSERT_ABORT
	gmp_abort();
#endif
	return 1;
}
