#ifndef _FILE_BUS_BASE_H
#define _FILE_BUS_BASE_H

// platform header
#include <comm/comm_basic.h>

class io_bus_base
	:public io_device_base
{
public:
	io_bus_base()
	{

	}

	~io_bus_base()
	{}

public:
	io_device_base* filter[];

};



#endif //_FILE_BUS_BASE_H
