// system include


// platform include
#include <./comm/physical_device_base.h>

#ifndef _FILE_BH1750_H_

class bh1750
	: record_dev
{
public:
	// UUID
	gmp_addr_t addr = 
#if ADDR_SEL = 0
		0x000
#else
		0x111
#endif // ADDR_SEL 
		;


public:
	// ctor & dtor
	bh1750()
	{
		addr = 0x000;
	}
public:

		
};

#endif // _FILE_BH1750_H_
