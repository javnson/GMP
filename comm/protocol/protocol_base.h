#ifndef _FILE_PROTOCOL_BASE_H
#define _FILE_PROTOCOL_BASE_H

// platform header
#include <comm/comm_basic.h>

class protocol_base
	:public io_device_base
{
public:
	protocol_base()
	{

	}

	~protocol_base()
	{}

public:
	io_device_base* filter;

};



#endif //_FILE_PROTOCOL_BASE_H