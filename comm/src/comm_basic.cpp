
// system include
//#include <memory.h>
#include <string.h>

// platforms
#include <./comm/comm_basic.h>


//////////////////////////////////////////////////////////////////////////
// cmd_device
#pragma region cmd_device_source
gmp_stat_t cmd_device::cmd(uint32_t cmd)
{

	//switch (cmd)
	//{
	//	case 
	//}



	//print unknown command
	error(DEVICE_ERR_UNKNOWN_CMD);

	// When the function are called, the command is an absolutely unknown command.
	return GMP_STAT_UNDEFINED_ACTION;
}

gmp_stat_t cmd_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	// print unknown command
	error(DEVICE_ERR_UNKNOWN_CMDEX);

	// When the function are called, the command is an absolutely unknown command.
	return GMP_STAT_UNDEFINED_ACTION;

}

// uint32_t cmd_device::get_device_usage_label()
// {
// 	return m_device_usage_label;
// }

// void cmd_device::clear_error_cnt()
// {
// //	m_error_cnt = 0;
// //	m_warn_cnt = 0;
// 	warn_cond = 0;
// }

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

gmp_diff_t io_device_base::read(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(m_dev != nullptr);
	gmp_assert(data != nullptr);


	// judge if the condition is meet

	// Condition 0: If any fatal error haven't been clear
	CHECK_ERROR_COND
		return  -1;

	// Condition 1: read the device is permitted.
	if (character.bits.read == DEV_CHAR_DISABLE) // read operation is not permitted.	
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_UNSUPPORT_R))
			return -1;
	}

	// Condition 2: Device is locked
	if (lock.bits.read != DEV_CHAR_LOCKED)
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_LOCKED))
			return -1;	
	}

	// Condition 3: Device is not yet ready
	if (m_state.bits.state_machine != DEVICE_STATE_READY)
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_NOT_READY))
			return  -1;		
	}

	//// Condition 4: Check if memory is useful
	//if (data == NULL)
	//{
	//	error(DEVICE_ERR_MEMORY_UNAVAILABLE);
	//	refuse(addr, data, length);
	//}

	// boundary case
	if (length == 0)
		return 0;


	return read_ex(addr, data, length);
}

gmp_diff_t io_device_base::write(_IN gmp_addr_t addr, _IN gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(data != nullptr);
	gmp_assert(m_dev != nullptr);

	// judge if the condition is meet

	// Condition 0: If any fatal error haven't been clear
	CHECK_ERROR_COND
		return  -1;

	// Condition 1: write the device is permitted.
	if (character.bits.write == 0) // write oper is not permitted.	
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_UNSUPPORT_W))
			return -1;
	}

	// Condition 2: Device is locked
	if (lock.bits.write != 0)
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_LOCKED))
			return -1;
	}

	// Condition 3: Device is not yet ready
	if (m_state.bits.state_machine != DEVICE_STATE_READY)
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_NOT_READY))
			return  -1;
	}

	// Condition 4: Check if memory is useful
	//if (data == NULL)
	//{
	//	error(DEVICE_ERR_MEMORY_UNAVAILABLE);
	//	refuse(addr, data, length);
	//}

	// boundary case
	// CHECK: is this safe
	if (length == 0)
		return 0;

	return write_ex(addr, data, length);

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


		break;
	case DEVICE_CMD_INIT:
		// no device are attached to the class
		if (m_dev == nullptr)
			return DEVICE_ERR_NULL_DEV_HANDLE;
		
		// check error condition
		CHECK_ERROR_COND
			return DEVICE_ERR_COND;

		// check if state machine is fulfilling the requirement.
		if (m_state.bits.state_machine == DEVICE_STATE_READY)
		// no need to init, quit
		{
			if (error(DEVICE_ERR_HAVE_INITED))
				return DEVICE_OK;
		}
		else if((m_state.bits.state_machine == DEVICE_STATE_SHUTDOWN) || 
			(m_state.bits.state_machine == DEVICE_STATE_CONFIG))

		{
			// Do nothing just change state machine.
			m_state.bits.state_machine = DEVICE_STATE_READY;
		}
		else // error state no need to init
		{
			if (error(DEVICE_ERR_BAD_CMD))
				return DEVICE_ERR_BAD_CMD;
		}
		break;
	case DEVICE_CMD_SHUTDOWN:
		// Call virtual function
		this->shutdown();
		break;
	case DEVICE_CMD_PNP_TEST:
		// do nothing

		break;
	case DEVICE_CMD_PNP_EJECT:
		// do nothing
		break;
	case DEVICE_CMD_NULL:
		// The last command named NULL command.
		// This command is used to test command reaction.
		if (m_state.bits.state_machine >= DEVICE_STATE_VERBOSE_2)
			gmp_print(_TEXT("[INFO] Device Command Null react!\r\n"));
		break;
	default:
		// return to cmd_device class to deal with
		cmd_device::cmd(cmd);
		break;
	}
	return GMP_STAT_OK;
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

gmp_stat_t io_device_base::preinit()
{
	return cmd(DEVICE_CMD_PREINIT);
}

gmp_stat_t io_device_base::ready()
{
	return cmd(DEVICE_CMD_READY);
}

gmp_stat_t io_device_base::reset()
{
	// step 1 judge if error happened, and clear the error.


	// Step 2 update state machine

	

	// If device has meets fatal error and re-initialize is necessary
	//if (m_state.bits.state_machine == DEVICE_STATE_RUNTIME_ERROR)
	//{
	//	m_state.bits.state_machine = DEVICE_STATE_READY;
	//	warning(DEVICE_RECOVER_FROM_ERROR);
	//}
	//// If device has meets runtime error and should clear error flag or error register.
	//else if (m_state.bits.state_machine == DEVICE_STATE_ERROR)
	//{
	//	m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
	//	warning(DEVICE_RECOVER_FROM_RUNTIME);
	//}
	return GMP_STAT_OK;
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

gmp_stat_t uart_device::cmd(uint32_t cmd)
{
	return io_device_base::cmd(cmd);
}

gmp_stat_t uart_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return io_device_base::cmd(cmd, wparam, lparam);
}

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
