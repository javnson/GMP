#ifndef _FILE_PROTOCOL_BASE_H
#define _FILE_PROTOCOL_BASE_H

// platform header
#include <comm/comm_base.h>


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
	// kernel virtual function
	// This class will implement the R/W functions.
	RESPONSE_RW

	// This class will implement the cmd function.
	RESPONSE_CMD

public:
	io_device_base* filter; // I2C bus, RS485 BUS

};



#endif //_FILE_PROTOCOL_BASE_H