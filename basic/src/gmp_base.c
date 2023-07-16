

#include <.\basic\gmp_base.h>


void gmp_startup(void)
{

}


// Default GMP abort function
__weak void gmp_abort_callback(void)
{
	// do nothing
}



void gmp_abort(void)
{
	// release GMP library resources

	// involke user abort function
	gmp_abort_callback();

	// endless loop
	while (1);
}
