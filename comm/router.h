// Router is a transfer class
// Router contains two objects one is input object and another is output object.

#include <comm/comm_base.h>

#ifndef _FILE_ROUTER_H_
#define _FILE_ROUTER_H_

class router
	: cmd_device
{
public:
	// ctor & dtor
	router()
		:m_input_dev(nullptr),m_output_dev(nullptr)
	{}

	router(io_device_base* input, io_device_base* output)
		:m_input_dev(input),m_output_dev(output)
	{}

	~router()
	{}

public:
	// command react
	RESPONSE_CMD

public:
	// utilities

	/**
	 * @brief This function is callback for the router.
	 * @param null.
	 * @return null
	 * @note User should call the function at a set interval 
	 * @author : Javnson
	 * @date   : 20230809
	 */
	virtual gmp_stat_t router_callback()
	{
		return GMP_STAT_OK;
	}

	io_device_base* get_input_dev()
	{
		return m_input_dev;
	}

	io_device_base* get_output_dev()
	{
		return m_output_dev;
	}

public:
	// kernel members
	io_device_base* m_input_dev;
	io_device_base* m_output_dev;
};



#endif // _FILE_ROUTER_H_

