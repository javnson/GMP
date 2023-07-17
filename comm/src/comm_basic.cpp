
// system include
//#include <memory.h>
#include <string.h>

// platforms
#include <./comm/comm_basic.h>


//////////////////////////////////////////////////////////////////////////
// cmd_device
gmp_stat_t cmd_device::cmd(uint32_t cmd)
{


	// When the function are called, the command is an absolutely unknown command.
	return GMP_STATUS_UNKNOWN_CMD;
}

gmp_stat_t cmd_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	// When the function are called, the command is an absolutely unknown command.
	return GMP_STATUS_UNKNOWN_CMD;

}

uint32_t cmd_device::get_device_usage_label()
{
	return m_device_usage_label;
}

void cmd_device::clear_error_cnt()
{
	m_error_cnt = 0;
	m_warn_cnt = 0;
	warn_cond = 0;
}

void cmd_device::set_verbose(uint8_t verbose)
{
	this->verbose = verbose;
}

void cmd_device::error(uint32_t errcode)
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

		m_warn_cnt += 1;
		return 1;
	}

	// Part 2 General error
	if(errcde >= DEVICE_ERRO_BEGIN) // error
	{
		if (get_verbose() >= DEVICE_STATE_VERBOSE_ERROR)
			gmp_print("[erro.%d]\t general ERROR happened, error code: %d.\r\n",
				g_info_cnt ,errcode);

		m_error_cnt += 1;
		m_last_error = errcode;
		return 1;
	}
	else if (errcode >= DEVICE_WARN_BEGIN) // warning
	{
		if (get_verbose() >= DEVICE_STATE_VERBOSE_WARNING)
			gmp_print("[warn.%d]\t general WARNING happened, warning code: %d.\r\n", 
				g_info_cnt, errcode);
		
		m_warn_cnt += 1;
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

void cmd_device::error_release()
{
	erro_cond = 0;
}

//////////////////////////////////////////////////////////////////////////
// io_device_base
gmp_diff_t io_device_base::read(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(m_dba != nullptr);
	gmp_assert(data != nullptr);


	// judge if the condition is meet

	// Condition 0: If any fatal error haven't been clear
	if (erro_cond)
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_COND))
			return -1;
	}

	// Condition 1: read the device is permitted.
	if (character.read == DEV_CHAR_DISABLE) // read operation is not permitted.	
	{
		refuse(addr, data, length);
		if (error(DEVICE_ERR_UNSUPPORT_R))
			return -1;
	}

	// Condition 2: Device is locked
	if (lock.read != DEV_CHAR_LOCKED)
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

gmp_ptrdiff_t io_device_base::write(_IN gmp_addr_t addr, _IN gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(data != nullptr);

	// boundary case
	// NOTE: is this safe
	if (length == 0)
		return 0;

	// judge if the condition is meet

	// Condition 1: write the device is permitted.
	if ((m_state.bits.characters & DEVICE_STATE_CHAR_W) == 0) // write oper is not permitted.	
	{
		error(DEVICE_ERR_UNSUPPORT_W);
		refuse(addr, data, length);
	}

	// Condition 2: Device is locked
	if (m_state.bits.lock != 0)
	{
		error(DEVICE_ERR_LOCKED);
		refuse(addr, data, length);
	}

	// Condition 3: Device is not yet ready
	if (m_state.bits.state_machine != DEVICE_STATE_READY)
	{
		warning(DEVICE_ERR_NOT_READY);
		refuse(addr, data, length);
	}

	// Condition 4: Check if memory is useful
	if (data == NULL)
	{
		error(DEVICE_ERR_MEMORY_UNAVAILABLE);
		refuse(addr, data, length);
	}

	return write_ex(addr, data, length);

}

void io_device_base::refuse(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	// print function 
	if (m_state.bits.verbose >= DEVICE_STATE_VERBOSE_2)
	{
		gmp_print(_TEXT("[INFO] The default refuse function has been called."));
	}
}

gmp_diff_t io_device_base::command(uint32_t cmd)
{
	// More emergency, more ahead
	switch (cmd)
	{
	case DEVICE_CMD_UNLOCK:
		//		this->unlock();
		break;
	case DEVICE_CMD_LOCK:
		//		this->lock();
		break;
	case DEVICE_CMD_RESET:
		// Call virtual function.
		this->reset();
		break;
	case DEVICE_CMD_INIT:
		// Call virtual function
		this->init();
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
		// push an unknown command warning.
		warning(DEVICE_ERR_UNKNOWN_CMD);
		break;
	}
	return GMP_STATUS_OK;
}

gmp_ptrdiff_t io_device_base::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	switch (cmd)
	{
	case DEVICE_CMD_VERBOSE:
		// wParam: verbose param, you may choose 0-3
		// lParam: reserve
		if (wparam > DEVICE_STATE_VERBOSE_3)
		{
			warning(DEIVCE_ERR_BAD_CMD);
		}
		else
		{
			m_state.bits.verbose = wparam;
		}
	case DEVICE_CMD_REGISTER:
		// wParam: device base address
		// TODO this function shouldn't be called another time.
		m_dba = (void*)wparam;
		break;
	case DEVICE_CMD_NULL:
		// The last command named null for command test
		gmp_print(_TEXT("[INFO] Device Command Null react!wParam:0x%08x,lParam:0x%08x\r\n"),
			wparam, lparam);
		break;
	default:
		// push an unknown message warning
		warning(DEVICE_ERR_UNKNOWN_CMDEX);
		break;
	}

	return GMP_STATUS_OK;
}

gmp_ptrdiff_t io_device_base::read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	memcpy((void*)addr, (void*)data, length);

	return length;
}

gmp_ptrdiff_t io_device_base::write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	memcpy((void*)data, (void*)addr, length);

	return length;
}

void io_device_base::init()
{
	// assert 
	gmp_assert(m_dba != NULL);

	// If device has been inited or device still has error, init function shouldn't be called.
	if (m_state.bits.state_machine != DEVICE_STATE_SHUTDOWN)
	{
		// warning : this function duplicated invoked
		warning(DEVICE_ERR_CANNOT_INIT);

		// ignore this warning
		return;
	}
	else
	{
		// Update device state machine
		m_state.bits.state_machine = DEVICE_STATE_READY;
	}

	// You should change state machine firstly, and then do init process.
}

void io_device_base::reset()
{
	// If device has meets fatal error and re-initialize is necessarys
	if (m_state.bits.state_machine == DEVICE_STATE_RUNTIME_ERROR)
	{
		m_state.bits.state_machine = DEVICE_STATE_READY;
		warning(DEVICE_RECOVER_FROM_ERROR);
	}
	// If device has meets runtime error and should clear error flag or error register.
	else if (m_state.bits.state_machine == DEVICE_STATE_ERROR)
	{
		m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
		warning(DEVICE_RECOVER_FROM_RUNTIME);
	}
}

void io_device_base::shutdown()
{
	m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
}


// SPI device

gmp_ptrdiff_t spi_device::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{

	return io_device_base::command(cmd, wparam, lparam);
}

gmp_ptrdiff_t spi_device::command(uint32_t cmd)
{

	return io_device_base::command(cmd);
}

gmp_ptrdiff_t uart_device::command(uint32_t cmd)
{
	return io_device_base::command(cmd);
}

gmp_ptrdiff_t uart_device::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return io_device_base::command(cmd, wparam, lparam);
}

gmp_ptrdiff_t i2c_device::command(uint32_t cmd)
{
	return io_device_base::command(cmd);
}

gmp_ptrdiff_t i2c_device::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return io_device_base::command(cmd, wparam, lparam);
}
