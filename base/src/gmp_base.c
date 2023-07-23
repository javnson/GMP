

#include <base\gmp_base.h>


uint32_t g_info_cnt = 0;
uint32_t g_warn_cnt = 0;
uint32_t g_erro_cnt = 0;


void gmp_startup(void)
{
	gmp_print("[INFO.%d]\t GMP print test done!\r\n");

}


// Default GMP abort function
__weak void gmp_abort_callback(void)
{
	// do nothing
}

// Default tick function : No tick, all sequential routine couldn't run
__weak gmp_size_t gmp_get_tick(void)
{
	return 0;
}



void gmp_abort(void)
{
	// release GMP library resources

	// invoke user abort function
	gmp_abort_callback();

	// endless loop
	while (1);
}
