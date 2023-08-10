
// system include
//#include <memory.h>
#include <string.h>

// platforms
#include <comm/comm_base.h>


//////////////////////////////////////////////////////////////////////////
// cmd_device

#pragma region cmd_device_source
gmp_stat_t cmd_device::cmd(uint32_t cmd)
{

	switch (cmd)
	{
	case DEVICE_CMD_NULL:
		// The last command named NULL command.
		// This command is used to test command reaction.
		if (m_state.bits.state_machine >= DEVICE_STATE_VERBOSE_2)
			gmp_print(_TEXT("[INFO] Device Command Null react!\r\n"));

		return DEVICE_OK;

	default:
		//print unknown command
		error(DEVICE_ERR_UNKNOWN_CMD);

		// When the function are called, the command is an absolutely unknown command.
		return GMP_STAT_UNDEFINED_ACTION;

	}
}

gmp_stat_t cmd_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{

	switch (cmd)
	{
	case DEVICE_CMD_NULL:
		// The last command named NULL command.
		// This command is used to test command reaction.
		if (m_state.bits.state_machine >= DEVICE_STATE_VERBOSE_2)
			gmp_print(_TEXT("[INFO] Device Command Null react!\r\n"));

		return DEVICE_OK;

	default:
		//print unknown command
		error(DEVICE_ERR_UNKNOWN_CMD);

		// When the function are called, the command is an absolutely unknown command.
		return GMP_STAT_UNDEFINED_ACTION;

	}
}


void cmd_device::set_verbose(uint8_t verbose)
{
	this->verbose = verbose;
}


gmp_fast_t cmd_device::error(uint32_t errcode)
{
	// NOTE return 1 to stop the process
	//      return 0 to ignore the process
	
	// global information counter
	g_info_cnt += 1;

	// Part 1 Some special code
		
	// Some fatal error has happened, so user should figure them out first.
	if (errcode == DEVICE_ERR_COND)
	{
		if (get_verbose() >= DEVICE_STATE_VERBOSE_ERROR)
			gmp_print("[erro.%d]\t device still in ERROR condition.\r\n",
				g_info_cnt);

		warn_cond = 1;
		g_warn_cnt += 1;

		return 1;
	}

	// Part 2 General error

	if(errcode >= DEVICE_ERRO_BEGIN) // error
	{
		if (get_verbose() >= DEVICE_STATE_VERBOSE_ERROR)
			gmp_print("[erro.%d]\t general ERROR happened, error code: %d.\r\n",
				g_info_cnt ,errcode);

		erro_cond = 1;
		g_erro_cnt += 1;

		m_last_error = errcode;
		return 1;
	}
	else if (errcode >= DEVICE_WARN_BEGIN) // warning
	{
		if (get_verbose() >= DEVICE_STATE_VERBOSE_WARNING)
			gmp_print("[warn.%d]\t general WARNING happened, warning code: %d.\r\n", 
				g_info_cnt, errcode);
		
		g_warn_cnt += 1;

		m_last_error = errcode;
		return 0;
	}
	else //info
	{
		if (get_verbose() >= DEVICE_STATE_VERBOSE_INFO)
			gmp_print("[info.%d]\t general INFO happened, info code: %d.\r\n",
				g_info_cnt, errcode);

		return 0;
	}


}


void cmd_device::clear_erro()
{
	erro_cond = 0;
}

#pragma endregion cmd_device_source

//////////////////////////////////////////////////////////////////////////
// io_device_base
#pragma region io_device_base_source

gmp_diff_t io_device_base::read(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity)
{
	gmp_assert(data != nullptr);
	//gmp_assert(m_dev != nullptr);


	// judge if the condition is meet
	if (!is_readable())
	{
		return -1;
	}

	// call virtual function and do read routine
	return read_ex(addr, data, capacity, 0);
}


gmp_diff_t io_device_base::write(_IN gmp_addr_t addr, _IN gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(data != nullptr);
	//gmp_assert(m_dev != nullptr);


	// judge if the condition is meet
	if (!is_writeable())
	{
		return -1;
	}

	// call virtual function and do write routine.
	return write_ex(addr, data, length);
}

gmp_diff_t read_write(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity, gmp_size_t length)
{
	gmp_assert(data != nullptr);
	//gmp_assert(m_dev != nullptr);


	// check character and lock state
	if (!is_readable())
		return -1;

	if (!is_writeable())
		return -1;

	// Call virtual function and do read_write routine.
	return read_ex(addr, data, capacity, length);

}



void io_device_base::refuse(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	// print function 
//	if (m_state.bits.verbose >= DEVICE_STATE_VERBOSE_2)
//	{
//		gmp_print(_TEXT("[INFO] The default refuse function has been called."));
//	}
}


gmp_stat_t io_device_base::cmd(uint32_t cmd)
{
	// More emergency, more ahead
	switch (cmd)
	{
	case DEVICE_CMD_UNLOCK:
		// just unlock all the routines without check
		lock.all = 0;
		break;
	case DEVICE_CMD_LOCK:
		// just lock all the routines without check
		lock = character;
		break;
	case DEVICE_CMD_RESET:
		// Step 0 User must do first
		// 1. User should stop any operation, so busy state will release.
		// 2. For any peripheral, user should clear all the error flag;
		//    or for any physical device, user should send reset signal. 
		// 3. For any software, user should clear software constrains.
		// And if user find any fatal error, user should return a non-zero value.
		
		// Step 1 update state machine
		if (erro_cond)
		{
			// check state machine
			if (m_state.bits.state_machine == DEVICE_STATE_READY)
			{
				m_state.bits.state_machine = DEVICE_STATE_CONFIG;
			}
			else if (m_state.bits.state_machine == DEVICE_STATE_CONFIG)
			{
				m_state.bits.state_machine = DEVICE_STATE_CONFIG;
			}
			else if (m_state.bits.state_machine == DEVICE_STATE_BUSY)
			{
				m_state.bits.state_machine = DEVICE_STATE_CONFIG;
			}
			else if (m_state.bits.state_machine == DEVICE_STATE_LOWPOWER)
			{
				m_state.bits.state_machine = DEVICE_STATE_CONFIG;
			}
			else //if (m_state.bits.state_machine == DEVICE_STATE_SHUTDOWN)
				;


		}

		// step 2 judge if error happened, and clear the error.
		erro_cond = 0;
		warn_cond = 0;

		break;
	case DEVICE_CMD_INIT:
		

		break;
	case DEVICE_CMD_SHUTDOWN:
		

		break;
	case DEVICE_CMD_PNP_TEST:
		// do nothing

		break;
	case DEVICE_CMD_PNP_EJECT:
		// do nothing
		break;
	}

	return cmd_device::cmd(cmd);
}


gmp_stat_t io_device_base::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	switch (cmd)
	{
	case DEVICE_CMD_VERBOSE:
		// wParam: verbose param, you may choose 0-3
		// lParam: reserve
		if (wparam > DEVICE_STATE_VERBOSE_3)
		{
			cmd_device::error(DEVICE_ERR_BAD_CMD);
			//warning(DEIVCE_ERR_BAD_CMD);

		}
		else
		{
			this->verbose = wparam;
		}
	case DEVICE_CMD_REGISTER:
		// wParam: device base address
		// TODO this function shouldn't be called another time.
		m_dev = (void*)wparam;
		break;
	case DEVICE_CMD_NULL:
		// The last command named null for command test
		gmp_print(_TEXT("[INFO] Device Command Null react!wParam:0x%08x,lParam:0x%08x\r\n"),
			wparam, lparam);
		break;
	default:
		// return to cmd_device class to deal with
		cmd_device::cmd(cmd);
		break;
	}

	return GMP_STAT_OK;
}


gmp_diff_t io_device_base::read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	memcpy((gmp_data_t*)m_dev + addr, (gmp_data_t*)data, length);

	return length;
}


gmp_diff_t io_device_base::write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	memcpy((gmp_data_t*)data, (gmp_data_t*)m_dev + addr, length);

	return length;
}

gmp_stat_t io_device_base::init()
{
	// call a cmd to solve the problem

	return cmd(DEVICE_CMD_INIT);


	// assert 
// 	gmp_assert(m_dev != NULL);

	// If device has been inited or device still has error, init function shouldn't be called.
// 	if (m_state.bits.state_machine != DEVICE_STATE_SHUTDOWN)
// 	{
		// warning : this function duplicated invoked
// 		warning(DEVICE_ERR_CANNOT_INIT);

		// ignore this warning
//		return;
//	}
//	else
//	{
		// Update device state machine
//		m_state.bits.state_machine = DEVICE_STATE_READY;
//	}

	// You should change state machine firstly, and then do init process.
}

gmp_stat_t io_device_base::enable()
{
	return cmd(DEVICE_CMD_ENABLE);
}

gmp_stat_t io_device_base::ready()
{
	// Check if in error state
	CHECK_ERROR_COND
		return  DEVICE_ERR_COND;


	// Check state machine.
	if (m_state.bits.state_machine == DEVICE_STATE_SHUTDOWN)
		return cmd(DEVICE_CMD_SETUP);
	else if (m_state.bits.state_machine == DEVICE_STATE_CONFIG)
		return cmd(DEVICE_CMD_INIT);
	else if (m_state.bits.state_machine == DEVICE_STATE_READY)
		return DEVICE_OK;
	else if (m_state.bits.state_machine == DEVICE_STATE_BUSY)
	{
		if (error(DEVICE_ERR_CANNOT_INIT))
			return DEVICE_ERR_CANNOT_INIT;
	}

	return DEVICE_ERR_CANNOT_INIT;
}

gmp_stat_t io_device_base::reset()
{
	return cmd(DEVICE_CMD_RESET);
}

gmp_stat_t io_device_base::shutdown()
{
	m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
	return GMP_STAT_OK;
}

#pragma endregion io_device_base_source

//////////////////////////////////////////////////////////////////////////
// SPI device


gmp_stat_t spi_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{

	return io_device_base::cmd(cmd, wparam, lparam);
}

gmp_stat_t spi_device::cmd(uint32_t cmd)
{

	return io_device_base::cmd(cmd);
}


//////////////////////////////////////////////////////////////////////////
// UART device

gmp_stat_t uart_device::cmd(uint32_t cmd)
{
	return io_device_base::cmd(cmd);
}

gmp_stat_t uart_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return io_device_base::cmd(cmd, wparam, lparam);
}


//////////////////////////////////////////////////////////////////////////
// IIC device

gmp_stat_t i2c_device::cmd(uint32_t cmd)
{
	return io_device_base::cmd(cmd);
}


gmp_stat_t i2c_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return io_device_base::cmd(cmd, wparam, lparam);
}

gmp_diff_t i2c_device::read_ex(gmp_addr_t addr, gmp_data_t* data, gmp_size_t length)
{
	return GMP_STAT_OK;
}

gmp_diff_t i2c_device::write_ex(gmp_addr_t addr, gmp_data_t* data, gmp_size_t length)
{
	return GMP_STAT_OK;
}

//////////////////////////////////////////////////////////////////////////
// GPIO device
gmp_stat_t gpio_dev::set(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_set(port_group,gpio_index);
}

gmp_stat_t gpio_dev::reset(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_reset(port_group,gpio_index);
}

gmp_stat_t gpio_dev::toggle(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_toggle(port_group,gpio_index);
}

int8_t gpio_dev::read(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_read(port_group,gpio_index);
}




