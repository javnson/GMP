#include "comm_stm32.h"
#include "stm32l1xx_hal.h"

gmp_ptrdiff_t spi_device_stm32::command(uint32_t cmd)
{
	// Get device base address
	SPI_TypeDef* instance = (SPI_TypeDef*)m_dba;
	// Get device handle
	// m_handle

	// Do not react default case!
	switch (cmd)
	{
		// realtime react function


		// pheripheral config 
	case DEVICE_CMD_INIT:
		init();
		return GMP_STATUS_OK;
	case DEVICE_CMD_RESET:
		reset();
		return GMP_STATUS_OK;
	case DEVICE_CMD_SHUTDOWN:
		shutdown();
		return GMP_STATUS_OK;

	}

	// Call parent class to react the command.
	return spi_device::command(cmd);

}

gmp_ptrdiff_t spi_device_stm32::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	// Get device base address
	SPI_TypeDef* instance = (SPI_TypeDef*)m_dba;
	// Get device handle
	// m_handle

	// Do not react default case!
	switch (cmd)
	{
		// realtime command


		// pheripheral config
	case DEVICE_CMD_REGISTER:
		// wParam: device base address
		// lParam: device abstract handle
		reg_device((SPI_TypeDef*)wparam, (SPI_HandleTypeDef*)lparam);
		return GMP_STATUS_OK;

	}

	return spi_device::command(cmd, wparam, lparam);
}

void spi_device_stm32::reg_device(SPI_TypeDef* spi_channel, SPI_HandleTypeDef* spi_handle)
{
	m_dba = spi_channel;
	m_handle = spi_handle;
}

#ifdef GMP_INIT_DEPOSIT
void spi_device_stm32::init()
{
	// Call parents' init function firstly.
	spi_device::init();

	// assert handle
	gmp_assert(NULL != m_handle);

	// If you is using CUBEMX code generation you shold using macro GMP_INIT_DEPOSIT.
	// This function would only change the state machine without any substantive operations.
	// If you need to restart the device, please call CubeMX Init function and MspInit function.
	// So as to make this function owns ability of substantive operations.
//	if (HAL_SPI_Init(m_handle) != HAL_OK)
//	{
//		error(DEIVCE_IVALID_OPER);
//		gmp_abort();
//	}

	// If you want bind you own board with peripheral, please derive the class and rewrite 
	// the init function. And finally unbind with CubeMX.

	// To prevent you missing any necessary initializing steps.
	// We are strongly sugggested you to organize your initial code as the following code.
	// 0. Preparing -> 1. Start pheripheral clock -> 
	// 2. GPIO multiplex -> 3. init peripheral handle

#if 0 // example code
	// NOTE: Step 0 preparing
	// Assert
	gmp_assert((m_dba != NULL) || (m_handle != NULL));

	// Get device base address
	SPI_TypeDef* instance = (SPI_TypeDef*)m_dba;
	// Get device handle
	// m_handle

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	if (instance == SPI6)
	{
		// NOTE: Step 1 Start peripheral clock
		// Initializes the peripherals clock
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
		PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_D3PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			error(DEIVCE_IVALID_OPER);
			gmp_abort();
		}

		// SPI6 clock enable 
		__HAL_RCC_SPI6_CLK_ENABLE();

		// NOTE: Step 2 Config peripheral GPIO multiplex
		// Enable GPIO clock
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**SPI6 GPIO Configuration
		PB3 (JTDO/TRACESWO)     ------> SPI6_SCK
		PB5                     ------> SPI6_MOSI
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}

	// NOTE: Step 3 Init peripheral handle
	m_handle->Instance = instance;
	m_handle->Init.Mode = SPI_MODE_MASTER;
	m_handle->Init.Direction = SPI_DIRECTION_1LINE;
	m_handle->Init.DataSize = SPI_DATASIZE_8BIT;
	m_handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
	m_handle->Init.CLKPhase = SPI_PHASE_2EDGE;
	m_handle->Init.NSS = SPI_NSS_SOFT;
	m_handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	m_handle->Init.FirstBit = SPI_FIRSTBIT_MSB;
	m_handle->Init.TIMode = SPI_TIMODE_DISABLE;
	m_handle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	m_handle->Init.CRCPolynomial = 0x0;
	m_handle->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	m_handle->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	m_handle->Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	m_handle->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	m_handle->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	m_handle->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	m_handle->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	m_handle->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	m_handle->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	m_handle->Init.IOSwap = SPI_IO_SWAP_DISABLE;

	if (HAL_SPI_Init(m_handle) != HAL_OK)
	{
		error(DEIVCE_IVALID_OPER);
		gmp_abort();
	}
#endif // Example code

	return;

}

void spi_device_stm32::reset()
{

}

void spi_device_stm32::shutdown()
{
	uint32_t cnt = 0;

	// wait until transmiting or receiving routine complete or timeup.
	while (++cnt < GMP_MAX_WAIT_CNT)
//		if ((HAL_SPI_GetState(m_handle) & SPI_FLAG_TXP) != 0)
			break;

	// if timeout, abort transmit.
	if (cnt >= GMP_MAX_WAIT_CNT)
		HAL_SPI_Abort(m_handle);

	// De-initialize SPI peripheral
//	HAL_SPI_DeInit(m_handle);

	// call parent shutdown function
	spi_device::shutdown();

}


#endif // GMP_CUBEMX_INIT_DEPOSIT

gmp_ptrdiff_t i2c_device_stm32::read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(data != 0);

	// Append R/W bit 1, reading 
	gmp_addr_t addr_a = ((addr << 1) + 1) & 0xFF;

	HAL_StatusTypeDef status = HAL_I2C_Master_Receive(m_handle, addr_a, data, length, GMP_MAX_WAIT_MS);

	if (status == HAL_OK)
	{
		return length;
	}
	else if (status == HAL_BUSY)
	{
		warning(DEVICE_ERR_TIMEOUT);
		return 0;
	}
	else if (status == HAL_ERROR) 
	{
		warning(DEVICE_ERR_HAL);
		return 0;
	}
	return length;
}

gmp_ptrdiff_t i2c_device_stm32::write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{
	gmp_assert(data != 0);

	// Append R/W bit 0, writing
	gmp_addr_t addr_a = (addr << 1) & 0xFF;


	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(m_handle, addr_a, data, length, GMP_MAX_WAIT_MS);

	if (status == HAL_OK)
	{
		return length;
	}
	else if (status == HAL_BUSY)
	{
		warning(DEVICE_ERR_TIMEOUT);
		return 0;
	}
	else if (status == HAL_ERROR) 
	{
		warning(DEVICE_ERR_HAL);
		return 0;
	}
	return length;
}

gmp_ptrdiff_t i2c_device_stm32::command(uint32_t cmd)
{
	return i2c_device::command(cmd);
}

gmp_ptrdiff_t i2c_device_stm32::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return i2c_device::command(cmd,wparam,lparam);
}
