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

	// implement a set of cmd 
	// startup
	// get result-CMD
	// accuracy
	// duration


public:
	io_device_base* filter; // I2C bus, RS485 BUS

};



#endif //_FILE_PROTOCOL_BASE_H