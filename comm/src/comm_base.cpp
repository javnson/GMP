
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
	case GMPDEV_CMD_NULL:
		// The last command named NULL command.
		// This command is used to test command reaction.
		if (m_state.bits.state_machine >= GMPDEV_STAT_VERBOSE_2)
			gmp_print(_TEXT("[INFO] Device Command Null react!\r\n"));

		return GMPDEV_OK;

	case GMPDEV_CMD_VERBOSE:
		set_verbose(DEV_STAT_VERBOSE_2);
	default:
		//print unknown command
		error(GMPDEV_ERR_UNKNOWN_CMD);

		// When the function are called, the command is an absolutely unknown command.
		return GMP_STAT_UNDEFINED_ACTION;

	}
}

gmp_stat_t cmd_device::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{

	switch (cmd)
	{
	case GMPDEV_CMD_NULL:
		// The last command named NULL command.
		// This command is used to test command reaction.
		if (m_state.bits.state_machine >= GMPDEV_STAT_VERBOSE_2)
			gmp_print(_TEXT("[INFO] Device Command Null react!\r\n"));

		return GMPDEV_OK;
	case GMPDEV_CMD_VERBOSE:
		// wParam: verbose param, you may choose 0-3
		// lParam: reserve
		if (wparam > DEV_STAT_VERBOSE_3)
		{
			cmd_device::error(GMPDEV_ERR_BAD_CMD);
			//warning(DEIVCE_ERR_BAD_CMD);
			if (error(GMPDEV_ERR_BAD_CMD))
				return GMPDEV_ERR_BAD_CMD;
		}
		else
		{
			set_verbose(wparam);
		}
		
		return GMPDEV_OK;

	default:
		//print unknown command
		error(GMPDEV_ERR_UNKNOWN_CMD);

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
	if (errcode == GMPDEV_ERR_COND)
	{
		if (get_verbose() >= GMPDEV_STAT_VERBOSE_ERROR)
			gmp_print("[erro.%d]\t device still in ERROR condition.\r\n",
				g_info_cnt);

		warn_cond = 1;
		g_warn_cnt += 1;

		return 1;
	}

	// Part 2 General error

	if (errcode >= DEVICE_ERRO_BEGIN) // error
	{
		if (get_verbose() >= GMPDEV_STAT_VERBOSE_ERROR)
			gmp_print("[erro.%d]\t general ERROR happened, error code: %d.\r\n",
				g_info_cnt, errcode);

		erro_cond = 1;
		g_erro_cnt += 1;

		m_last_error = errcode;
		return 1;
	}
	else if (errcode >= DEVICE_WARN_BEGIN) // warning
	{
		if (get_verbose() >= GMPDEV_STAT_VERBOSE_WARNING)
			gmp_print("[warn.%d]\t general WARNING happened, warning code: %d.\r\n",
				g_info_cnt, errcode);

		g_warn_cnt += 1;

		m_last_error = errcode;
		return 0;
	}
	else //info
	{
		if (get_verbose() >= GMPDEV_STAT_VERBOSE_INFO)
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


gmp_diff_t io_device_base::read_write(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity, gmp_size_t length)
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



//void io_device_base::refuse(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
//{
	// print function 
//	if (m_state.bits.verbose >= GMPDEV_STAT_VERBOSE_2)
//	{
//		gmp_print(_TEXT("[INFO] The default refuse function has been called."));
//	}
//}


gmp_stat_t io_device_base::cmd(uint32_t cmd)
{
	// More emergency, more ahead
	switch (cmd)
	{
	case GMPDEVCMD_UNLOCK:
#if defined GMP_DEV_DISABLE_LOCK
		if (error(GMPDEV_ERR_UNSUPPORT_LOCK))
			return GMPDEV_ERR_UNSUPPORT_LOCK;
#else
		// just unlock all the routines without check
		lock.all = 0;
#endif
		return GMPDEV_OK;

	case GMPDEVCMD_LOCK:
#if defined GMP_DEV_DISABLE_LOCK
		if (error(GMPDEV_ERR_UNSUPPORT_LOCK))
			return GMPDEV_ERR_UNSUPPORT_LOCK;
#else
		// just lock all the characters without check
		lock = character;
#endif
		return GMPDEV_OK;

	case GMPDEVCMD_RESET:
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
			if (m_state.bits.state_machine == GMPDEV_STAT_SHUTDOWN)
			{
				//m_state.bits.state_machine = GMPDEV_STAT_SHUTDOWN;
				// NO NEED TO WRITE DIRECTLY.
			}
			else
			{
				m_state.bits.state_machine = GMPDEV_STAT_CONFIG;
			}

		}

		// step 2 judge if error happened, and clear the error counter.
		erro_cond = 0;
		warn_cond = 0;

		return GMPDEV_OK;

	case GMPDEVCMD_READY:
		// step 0 user should complete all the preparing firstly.

		// step 1 judge if device still in error state. 
		if (erro_cond)
			error(GMPDEV_ERR_STILL_ERRO_COND);

		// step 2 judge if meets device state machine trans condition
		if (m_state.bits.state_machine == GMPDEV_STAT_CONFIG ||
			m_state.bits.state_machine == GMPDEV_STAT_SHUTDOWN ||
			m_state.bits.state_machine == GMPDEV_STAT_LOWPOWER)
			m_state.bits.state_machine = GMPDEV_STAT_READY;
		else if (m_state.bits.state_machine == GMPDEV_STAT_READY)
		{
			if (error(GMPDEV_ERR_STATE_NOT_CHG))
				return GMPDEV_ERR_STATE_NOT_CHG;
		}
		else if (m_state.bits.state_machine == GMPDEV_STAT_BUSY)
		{
			if (error(GMPDEV_ERR_BUSY))
				return GMPDEV_ERR_BUSY;
		}
		else
		{
			if (error(GMPDEV_ERR_CANNOT_READY))
				return GMPDEV_ERR_CANNOT_READY;
		}

		return GMPDEV_OK;

	case GMPDEVCMD_ENABLE:
		// step 0 User should startup the peripheral firstly

		// step 1 judge if device still in error state
		if (erro_cond)
			error(GMPDEV_ERR_STILL_ERRO_COND);

		// step 2 judge if meets device state machine trans condition
		if (m_state.bits.state_machine == GMPDEV_STAT_SHUTDOWN ||
			m_state.bits.state_machine == GMPDEV_STAT_LOWPOWER)
			m_state.bits.state_machine = GMPDEV_STAT_CONFIG;
		else if (m_state.bits.state_machine == GMPDEV_STAT_CONFIG ||
			m_state.bits.state_machine == GMPDEV_STAT_READY)
		{
			if (error(GMPDEV_ERR_STATE_NOT_CHG))
				return GMPDEV_ERR_STATE_NOT_CHG;
		}
		else if (m_state.bits.state_machine == GMPDEV_STAT_BUSY)
		{
			if (error(GMPDEV_ERR_BUSY))
				return GMPDEV_ERR_BUSY;
		}
		else
		{
			if (error(GMPDEV_ERR_CANNOT_ENABLE))
				return GMPDEV_ERR_CANNOT_ENABLE;
		}

		return GMPDEV_OK;

	case GMPDEVCMD_SHUTDOWN:
		// step 0 User should stop the device firstly.

		// step 1 judge if device still in error state
		if (erro_cond)
			error(GMPDEV_ERR_STILL_ERRO_COND);

		// step 2 judge if meets device state machine trans condition
		if (m_state.bits.state_machine == GMPDEV_STAT_CONFIG ||
			m_state.bits.state_machine == GMPDEV_STAT_READY)
			m_state.bits.state_machine = GMPDEV_STAT_SHUTDOWN;
		else if (m_state.bits.state_machine == GMPDEV_STAT_SHUTDOWN)
		{
			if (error(GMPDEV_ERR_STATE_NOT_CHG))
				return GMPDEV_ERR_STATE_NOT_CHG;
		}
		else if (m_state.bits.state_machine == GMPDEV_STAT_BUSY)
		{
			if (error(GMPDEV_ERR_BUSY))
				return GMPDEV_ERR_BUSY;
		}
		else
		{
			if (error(GMPDEV_ERR_CANNOT_SHUTDOWN))
				return GMPDEV_ERR_CANNOT_SHUTDOWN;
		}

		return GMPDEV_OK;

	case GMPDEVCMD_SUSPEND:
		// step 0 user should do something to prepare config mode.

		// step 1 judge if device still in error state
		if (erro_cond)
			error(GMPDEV_ERR_STILL_ERRO_COND);

		// step 2 judge if meets device state machine trans condition
		if (m_state.bits.state_machine == GMPDEV_STAT_READY)
			m_state = GMPDEV_STAT_CONFIG;
		else if (m_state.bits.state_machine == GMPDEV_STAT_CONFIG)
		{
			if (error(GMPDEV_ERR_STATE_NOT_CHG))
				return GMPDEV_ERR_STATE_NOT_CHG;
		}
		else if (m_state.bits.state_machine == GMPDEV_STAT_BUSY)
		{
			if (error(GMPDEV_ERR_BUSY))
				return GMPDEV_ERR_BUSY;
		}
		else
		{
			if (error(GMPDEV_ERR_CANNOT_SUSPEND))
				return GMPDEV_ERR_CANNOT_SUSPEND;
		}

		return GMPDEV_OK;

	case GMPDEVCMD_STANDBY:
		// step 0 user should do something to prepare config mode.

		// step 1 judge if device still in error state
		if (erro_cond)
			error(GMPDEV_ERR_STILL_ERRO_COND);

		// step 2 judge if meets device state machine trans condition
		if (m_state.bits.state_machine == GMPDEV_STAT_READY ||
			m_state.bits.state_machine == GMPDEV_STAT_CONFIG)
			m_state.bits.state_machine = GMPDEV_STAT_LOWPOWER;
		else if (m_state.bits.state_machine == GMPDEV_STAT_LOWPOWER)
		{
			if (error(GMPDEV_ERR_STATE_NOT_CHG))
				return GMPDEV_ERR_STATE_NOT_CHG;
		}
		else if (m_state.bits.state_machine == GMPDEV_STAT_BUSY)
		{
			if (error(GMPDEV_ERR_BUSY))
				return GMPDEV_ERR_BUSY;
		}
		else
		{
			if (error(GMPDEV_ERR_CANNOT_STANDBY))
				return GMPDEV_ERR_CANNOT_STANDBY;
		}

		return GMPDEV_OK;

	case GMPDEVCMD_PNP_TEST:
		// do nothing

		return GMPDEV_OK;
	case GMPDEVCMD_PNP_EJECT:
		// do nothing

		return GMPDEV_OK;
	}

	return cmd_device::cmd(cmd);
}


gmp_stat_t io_device_base::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	switch (cmd)
	{
	case GMPDEVCMD_REGISTER:
		// wParam: device base address
		// TODO this function shouldn't be called another time.
		m_dev = (void*)wparam;

		return DEVICE_OK;


	case GMPDEVCMD_STANDBY:
	case GMPDEVCMD_SUSPEND:
	case GMPDEVCMD_SHUTDOWN:
	case GMPDEVCMD_RESET:
	case GMPDEVCMD_READY:
	case GMPDEVCMD_ENABLE:

		// call the specified function without virtual
		return cmd_device::cmd(cmd);

	case GMPDEVCMD_PNP_TEST:
		// do nothing

		return GMPDEV_OK;
	case GMPDEVCMD_PNP_EJECT:
		// do nothing

		return GMPDEV_OK;
	}

	return cmd_device::cmd(cmd, wparam, lparam);
}


gmp_diff_t io_device_base::read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity, gmp_size_t length)
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

	return cmd(GMPDEV_CMD_INIT);


	// assert 
// 	gmp_assert(m_dev != NULL);

	// If device has been inited or device still has error, init function shouldn't be called.
// 	if (m_state.bits.state_machine != GMPDEV_STAT_SHUTDOWN)
// 	{
		// warning : this function duplicated invoked
// 		warning(DEVICE_ERR_CANNOT_INIT);

		// ignore this warning
//		return;
//	}
//	else
//	{
		// Update device state machine
//		m_state.bits.state_machine = GMPDEV_STAT_READY;
//	}

	// You should change state machine firstly, and then do init process.
}

gmp_stat_t io_device_base::enable()
{
	return cmd(GMPDEV_CMD_ENABLE);
}

gmp_stat_t io_device_base::ready()
{
	return cmd(GMPDEV_CMD_READY);
	//// Check if in error state
	//CHECK_ERROR_COND
	//	return  GMPDEV_ERR_COND;


	//// Check state machine.
	//if (m_state.bits.state_machine == GMPDEV_STAT_SHUTDOWN)
	//	return cmd(GMPDEV_CMD_SETUP);
	//else if (m_state.bits.state_machine == GMPDEV_STAT_CONFIG)
	//	return cmd(GMPDEV_CMD_INIT);
	//else if (m_state.bits.state_machine == GMPDEV_STAT_READY)
	//	return GMPDEV_OK;
	//else if (m_state.bits.state_machine == GMPDEV_STAT_BUSY)
	//{
	//	if (error(GMPDEV_ERR_CANNOT_INIT))
	//		return GMPDEV_ERR_CANNOT_INIT;
	//}

	//return GMPDEV_ERR_CANNOT_INIT;
}

gmp_stat_t io_device_base::reset()
{
	return cmd(GMPDEV_CMD_RESET);
}

gmp_stat_t io_device_base::shutdown()
{
	return cmd(GMPDEV_CMD_SHUTDOWN);
}

gmp_stat_t io_device_base::suspend()
{
	return cmd(GMPDEV_CMD_SUSPEND);
}

gmp_fast_t io_device_base::is_readable()

{
	// Condition 0: If any fatal error haven't been clear
	CHECK_ERROR_COND
		return  0;

	// Condition 1: read the device is permitted.
	if (character.bits.read == DEV_CHAR_DISABLE) // read operation is not permitted.	
	{
		refuse(addr, data, length);
		if (error(GMPDEV_ERR_UNSUPPORT_R))
			return 0;
	}

	// Condition 2: Device is locked
	if (lock.bits.read != DEV_CHAR_LOCKED)
	{
		refuse(addr, data, length);
		if (error(GMPDEV_ERR_LOCKED))
			return 0;
	}

	// Condition 3: Device is not yet ready
	if (m_state.bits.state_machine != GMPDEV_STAT_READY)
	{
		refuse(addr, data, length);
		if (error(GMPDEV_ERR_NOT_READY))
			return  0;
	}

	return 1;
}


gmp_fast_t io_device_base::is_writeable()
{
	// Condition 0: If any fatal error haven't been clear
	CHECK_ERROR_COND
		return  0;

	// Condition 1: write the device is permitted.
	if (character.bits.write == 0) // write oper is not permitted.	
	{
		refuse(addr, data, length);
		if (error(GMPDEV_ERR_UNSUPPORT_W))
			return 0;
	}

	// Condition 2: Device is locked
	if (lock.bits.write != 0)
	{
		refuse(addr, data, length);
		if (error(GMPDEV_ERR_LOCKED))
			return 0;
	}

	// Condition 3: Device is not yet ready
	if (m_state.bits.state_machine != GMPDEV_STAT_READY)
	{
		refuse(addr, data, length);
		if (error(GMPDEV_ERR_NOT_READY))
			return 0;
	}
	return 1;
}


//gmp_stat_t io_device_base::chg_dev_stat(uint8_t target_stat)
//{
//	// call state machine change callback 
//
//
//	// the target state
//	// NOTE: Sort by execution priority
//	switch (target_stat)
//	{
//	case GMPDEV_STAT_BUSY:
//		if (m_state == GMPDEV_STAT_READY)
//			m_state = GMPDEV_STAT_BUSY;
//		else if (m_state == GMPDEV_STAT_BUSY)
//		{
//			if (error(GMPDEV_ERR_STATE_NOT_CHG))
//				return GMPDEV_ERR_STATE_NOT_CHG;
//		}
//		else
//		{
//			if (error(GMPDEV_ERR_NOT_READY))
//				return GMPDEV_ERR_NOT_READY;
//		}
//		return GMPDEV_OK;
//
//	case GMPDEV_STAT_READY:
//		if (m_state == GMPDEV_STAT_SHUTDOWN ||
//			m_state == GMPDEV_STAT_CONFIG ||
//			m_state == GMPDEV_STAT_LOWPOWER)
//			m_state = GMPDEV_STAT_READY;
//		else if (m_state == GMPDEV_STAT_READY)
//		{
//			if (error(GMPDEV_ERR_STATE_NOT_CHG))
//				return GMPDEV_ERR_STATE_NOT_CHG;
//		}
//		else
//		{
//			if (error(GMPDEV_ERR_CANNOT_INIT))
//				return GMPDEV_ERR_CANNOT_INIT;
//		}
//
//		return GMPDEV_OK;
//
//	case GMPDEV_STAT_SHUTDOWN:
//		if (m_state == GMPDEV_STAT_READY ||
//			m_state == GMPDEV_STAT_CONFIG)
//			m_state = GMPDEV_STAT_SHUTDOWN;
//		else if (m_state == GMPDEV_STAT_SHUTDOWN)
//		{
//			if (error(GMPDEV_ERR_STATE_NOT_CHG))
//				return GMPDEV_ERR_STATE_NOT_CHG;
//		}
//		else if (m_state == GMPDEV_STAT_BUSY)
//		{
//			if (error(GMPDEV_ERR_BUSY))
//				return GMPDEV_ERR_BUSY;
//		}
//		else
//		{
//			if (error(GMPDEV_ERR_CANNOT_SHUTDOWN))
//				return GMPDEV_ERR_CANNOT_SHUTDOWN;
//		}
//
//		return GMPDEV_OK;
//
//	case GMPDEV_STAT_CONFIG:
//		if (m_state == GMPDEV_STAT_READY ||
//			m_state == GMPDEV_STAT_SHUTDOWN ||
//			m_state == GMPDEV_STAT_LOWPOWER)
//			m_state = GMPDEV_STAT_CONFIG;
//		else if (m_state == GMPDEV_STAT_CONFIG)
//		{
//			if (error(GMPDEV_ERR_STATE_NOT_CHG))
//				return GMPDEV_ERR_STATE_NOT_CHG;
//		}
//		else if(m_state == GMPDEV_STAT_BUSY)
//		{
//			if (error(GMPDEV_ERR_BUSY))
//				return GMPDEV_ERR_BUSY;
//		}
//		else
//		{
//			if (error(GMPDEV_ERR_CANNOT_CONFIG))
//				return GMPDEV_ERR_CANNOT_CONFIG;
//		}
//		
//		return GMPDEV_OK;
//
//	case GMPDEV_STAT_LOWPOWER:
//		if (m_state == GMPDEV_STAT_READY ||
//			m_state == GMPDEV_STAT_CONFIG)
//			m_state = GMPDEV_STAT_LOWPOWER;
//		else if (m_state = GMPDEV_STAT_LOWPOWER)
//		{
//			if (error(GMPDEV_ERR_STATE_NOT_CHG))
//				return GMPDEV_ERR_STATE_NOT_CHG;
//		}
//		else if (m_state == GMPDEV_STAT_BUSY)
//		{
//			if (error(GMPDEV_ERR_BUSY))
//				return GMPDEV_ERR_BUSY;
//		}
//		else
//		{
//			if (error(GMPDEV_ERR_CANNOT_CONFIG))
//				return GMPDEV_ERR_CANNOT_LOWPOWER;
//		}
//
//		return GMPDEV_OK;
//
//	default:
//		break;
//	}
//
//	return GMPDEV_ERR_UNKNOWN_STAT_TRANS;
//}

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
	return gmp_gpio_set(port_group, gpio_index);
}

gmp_stat_t gpio_dev::reset(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_reset(port_group, gpio_index);
}

gmp_stat_t gpio_dev::toggle(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_toggle(port_group, gpio_index);
}

int8_t gpio_dev::read(gmp_addr_t port_group, gmp_size_t gpio_index)
{
	return gmp_gpio_read(port_group, gpio_index);
}




